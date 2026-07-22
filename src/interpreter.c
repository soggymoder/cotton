// i got to eventually clean up this file a bit i feel like its very janky but i
// suppose i can worry about that later

#include "interpreter.h"
#include "cottonwindow.h"
#include "eiki.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// maximum length of a .cot file line
#define MAX_LINE 256

// da string AND variable stuff

static void 
strip_newline(char *s) 
{
     s[strcspn(s, "\r\n")] = 0; 
}

static char 
*get_arg (char *line)
{
    char *space = strchr(line, ' ');

    if (space != NULL) {
        return space + 1;
    }

    else {
        return NULL;
    }
}

static char 
*strip_quotes(char *s)
{
    if (s[0] == '"') {
        char *end = strrchr(s, '"');

        if (end == NULL) {
            fprintf(stderr, "cot syntax error : looks like you forgot a quote "
                            "at the end xP\n");
            return s;
        }

        *end = '\0';
        return s + 1;
    }

    return s;
}

static const char *get_var_value
(Cotton *cotton, const char *name)
{
    for (int i = 0; i < cotton->var_count; i++) {
        if (strcmp(cotton->vars[i].name, name) == 0) {
            return cotton->vars[i].value;
        }
    }
    return NULL;
}

// da graphics stuff

static void draw_pixel
(Cotton *cotton, int x, int y)
{
    cotton->video[y * VIDEO_WIDTH + x] = cotton->c_cottolette;
}

static void draw_char
(Cotton *cotton, char c, int x, int y)
{
    for (int row = 0; row < FONT_HEIGHT; row++) {
        uint8_t bits = EIKI_FONT[(uint8_t)c][row];

        for (int col = 0; col < FONT_WIDTH; col++) {

            if (bits & (1 << col)) {
                draw_pixel(cotton, x + col, y + row);
            }
        }
    }
}

// da commands

static void cmd_print
(Cotton *cotton, CottonWindow *cw, char *arg)
{
    if (!arg)
        return;

    char *text = strip_quotes(arg);

    const char *value = get_var_value(cotton, text);
    if (value != NULL) {
        text = (char *)value;
    }

    printf("%s\n", text);

    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            cotton->cursor_x = 0;
            cotton->cursor_y += FONT_HEIGHT + 4;
            continue;
        }

        draw_char(cotton, text[i], cotton->cursor_x, cotton->cursor_y);
        cotton->cursor_x += FONT_WIDTH + 2;

        if (cotton->cursor_x + FONT_WIDTH + 2 >= VIDEO_WIDTH) {
            cotton->cursor_x = 0;
            cotton->cursor_y += FONT_HEIGHT + 4;
        }
    }

    cotton->cursor_x = 0;
    cotton->cursor_y += FONT_HEIGHT + 4;

    cottonwindow_update(cw, cotton->video,
                        sizeof(cotton->video[0]) * VIDEO_WIDTH);
}

static void cmd_var
(Cotton *cotton, char *arg)
{
    if (!arg)
        return;

    char *space = strchr(arg, ' ');
    if (!space) {
        fprintf(stderr, "cot syntax error: you forgor to put a value for your "
                        "variable xP\n");
        return;
    }

    *space = '\0';
    char *name = arg;
    char *value = space + 1;

    cotton_store_var(cotton, name, value);
}

static void
cmd_wait(Cotton *cotton, char *arg)
{
    if (!arg)
        return;

    int seconds = atoi(arg);

    cotton->ticktock = SDL_GetTicks() + (seconds * 1000);
}

static void 
cmd_kill(void)
{
    printf("[cotton stopped]\n");
    exit(0);
}

static void cmd_clear(Cotton *cotton, CottonWindow *cw)
{
    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++) {
        cotton->video[i] = 0;
    }

    cotton->cursor_x = 0;
    cotton->cursor_y = 0;

    cottonwindow_update(cw, cotton->video,
                        sizeof(cotton->video[0]) * VIDEO_WIDTH);
}

static void 
cmd_color(Cotton *cotton, char *arg)
{
    if (!arg)
        return;

    if (strcmp(arg, "black") == 0)
        cotton->c_cottolette = COTTOLETTE[0];

    else if (strcmp(arg, "white") == 0)
        cotton->c_cottolette = COTTOLETTE[1];

    else if (strcmp(arg, "rey") == 0)
        cotton->c_cottolette = COTTOLETTE[2];

    else if (strcmp(arg, "wist") == 0)
        cotton->c_cottolette = COTTOLETTE[3];

    else if (strcmp(arg, "wink") == 0)
        cotton->c_cottolette = COTTOLETTE[4];

    else if (strcmp(arg, "laven") == 0)
        cotton->c_cottolette = COTTOLETTE[5];

    else if (strcmp(arg, "geen") == 0)
        cotton->c_cottolette = COTTOLETTE[6];

    else
        fprintf(stderr,
                "cot syntax error: cotton doesn't know the color \"%s\" "
                "sorry,, :(\n",
                arg);
}

static void 
cmd_input(Cotton *cotton, char *arg)
{
    if (!arg) {
        fprintf(stderr,
                "cotton syntax error: input needs a variable name xP\n");
        return;
    }

    char buffer[VAR_VAL_LEN];

    // kind of a quick and dirty way of implementing input, i know, but, i cant
    // be arsed to get myself into a potential rabbit hole with SDL so for now i
    // will leave this to be this way,, sorry,,
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';

        cotton_store_var(cotton, arg, buffer);
    }
}

static void 
cmd_unknown(const char *cmd)
{
    fprintf(stderr,
            "cot syntax error: cotton doesn't know what \"%s\" means, care to "
            "type that out again? :<\n",
            cmd);
}

// da interpreter loop

void 
cotton_interpret(Cotton *cotton, CottonWindow *cw, FILE *file)
{
    char line[MAX_LINE];

    // this reads the file one line at a time until we get to da end of da file
    if (!fgets(line, sizeof(line), file))
        return;

    strip_newline(line);

    // cot shall use # comments like C because C is cool and i love C so bwaaa
    if (line[0] == '\0' || line[0] == '#')
        return;

    char line_copy[MAX_LINE];

    strncpy(line_copy, line, MAX_LINE - 1);
    line_copy[MAX_LINE - 1] = '\0';

    char *cmd = strtok(line_copy, " ");

    if (!cmd)
        return;

    // RELEASE THE KRAKE- I MEAN, RELEASE THE GREAT WALL OF COMMANDS !!!
    // could i format this better? maybe,, but but i'd say this is readable and
    // i think readability is gud so i will keep it nice and green,,, green!! :D
    char *arg = get_arg(line);

    if (strcmp(cmd, "print") == 0) {
        cmd_print(cotton, cw, arg);
    }

    else if (strcmp(cmd, "var") == 0) {
        cmd_var(cotton, arg);
    }

    else if (strcmp(cmd, "clear") == 0) {
        cmd_clear(cotton, cw);
    }

    else if (strcmp(cmd, "color") == 0) {
        cmd_color(cotton, arg);
    }

    else if (strcmp(cmd, "wait") == 0) {
        cmd_wait(cotton, arg);
    }

    else if (strcmp(cmd, "input") == 0) {
        cmd_input(cotton, arg);
    }

    else if (strcmp(cmd, "kill") == 0) {
        cmd_kill();
    }

    else {
        cmd_unknown(cmd);
    }
}
