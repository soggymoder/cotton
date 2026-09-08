// i got to eventually clean up this file a bit i feel like its very janky but i suppose i can worry about that later - future dani here, it seems you ended up just remaking the entire file bc you thought remaking cot into a concatenated language was cooler, good job dani - future future dani here, thank you dani :D

#include "interpreter.h"
#include "cottonwindow.h"
#include "eiki.h"
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 256

// da stack 
static void push
(Cotton *c, int v)
{
    if (c->stack_p >= 64) {
        fprintf(stderr, "cot: there has been a stack overflow :< (like the website badumptsss)\n");
        return;
    }

    c->stack[c->stack_p++] = v;
}

static int pop
(Cotton *c)
{
    if (c->stack_p <= 0) {
        fprintf(stderr, "cot: there has been a stack underflow :<\n");
        return 0;
    }

    return c->stack[--c->stack_p];
}

// da variable stuff
static int find_var
(Cotton *c, const char *name)
{
    for (int i = 0; i < c->var_count; i++) {
        if (strcmp(c->vars[i].name, name) == 0) return i;
    }

    return -1;
}

static void add_var
(Cotton *c, const char *name)
{
    if (c->var_count >= VARS) {
        fprintf(stderr, "cot: too many variables\n");
        return;
    }

    strncpy(c->vars[c->var_count].name, name, VAR_NAME_LEN - 1);
    c->vars[c->var_count].name[VAR_NAME_LEN - 1] = '\0';
    strcpy(c->vars[c->var_count].value, "0");
    c->var_count++;
}

// da graphics
static void draw_pixel_c
(Cotton *c, int x, int y)
{
   	if (x < 0 || x >= VIDEO_WIDTH || y < 0 || y >= VIDEO_HEIGHT) return;
    c->video[y * VIDEO_WIDTH + x] = c->c_cottolette;
}

static void draw_char_c
(Cotton *c, char ch, int x, int y)
{
    for (int row = 0; row < FONT_HEIGHT; row++) {
        uint8_t bits = EIKI_FONT[(uint8_t)ch][row];

        for (int col = 0; col < FONT_WIDTH; col++) {
            if (bits & (1 << col)) {
                draw_pixel_c(c, x + col, y + row);
            }
        }
    }
}

// those who interpretate skull emoji skull emoji nahhhh get outttttttttt skull emoji door emoji
void cotton_interpret
(Cotton *c, CottonWindow *cw, FILE *file)
{
    char line[MAX_LINE];
    if (!fgets(line, sizeof(line), file)) return;
    
    line[strcspn(line, "\r\n")] = 0;
    if (line[0] == '\0' || (line[0] == '/' && line[1] == '/')) return;
    
    char *tok = strtok(line, " ");
    
    while (tok) {

        int is_a_num = 1;
        for (int i = 0; tok[i]; i++) {
            if (!isdigit(tok[i]) && !(i == 0 && tok[i] == '-')) {
                is_a_num = 0;
                break;
            }
        }
        
        if (is_a_num) {
            push(c, atoi(tok));
        }

		// those who do math..., math from the epic video gaem baldi's basics in education and learning,., you guys should play it its a good game i like baldi,. send me an email telling me your guys' PR on endless mode 
        else if (strcmp(tok, "+") == 0) {
            int b = pop(c), a = pop(c);
            push(c, a + b);
        }

        else if (strcmp(tok, "-") == 0) {
            int b = pop(c), a = pop(c);
            push(c, a - b);
        }

        else if (strcmp(tok, "*") == 0) {
            int b = pop(c), a = pop(c);
            push(c, a * b);
        }

        else if (strcmp(tok, "/") == 0) {
            int b = pop(c), a = pop(c);
            push(c, a / b);
        }

		// while forth does use ! and @, im quirky and #notlikeothergirls so im changing ! to -> bc i think it makes more sense :P
        else if (strcmp(tok, "->") == 0) {

            int slot = pop(c);
			int num = pop(c);
			
			if (slot >= 0 && slot < c->var_count) {
                char buf[VAR_VAL_LEN];
                sprintf(buf, "%d", num);
                strncpy(c->vars[slot].value, buf, VAR_VAL_LEN - 1);
			} 	

			else {
                fprintf(stderr, "cot: tried to store somewhere that don't exist.,. its in the void now.., :<\n");
            }
        }

        else if (strcmp(tok, "@") == 0) {
            int slot = pop(c);

        if (slot >= 0 && slot < c->var_count) {
                push(c, atoi(c->vars[slot].value));
            } 

        else {
        	fprintf(stderr, "cot: tried to fetch from somewhere that don't exist,., :<\n");
       	}

        }

        else if (strcmp(tok, ".") == 0) {
            printf("%d", pop(c));
            fflush(stdout);
        }

        else if (strcmp(tok, "var") == 0) {
            tok = strtok(NULL, " ");
            if (tok) add_var(c, tok);
        }

        else if (strcmp(tok, "pixel") == 0) {
            int y = pop(c);
            int x = pop(c);
            draw_pixel_c(c, x, y);
            cottonwindow_update(cw, c->video, sizeof(c->video[0]) * VIDEO_WIDTH);
        }

        else if (strcmp(tok, "color") == 0) {
            int slot = pop(c);
            if (slot >= 0 && slot < COTTOLETTE_SIZE)
                c->c_cottolette = COTTOLETTE[slot];
        }

        else if (strcmp(tok, "clear") == 0) {
            for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++) 
                c->video[i] = 0;
            c->cursor_x = 0;
            c->cursor_y = 0;
            cottonwindow_update(cw, c->video, sizeof(c->video[0]) * VIDEO_WIDTH);
        }

		// the great wall of colors
        else if (strcmp(tok, "blabla") == 0) push(c, 0);
        else if (strcmp(tok, "whie") == 0) push(c, 1);
        else if (strcmp(tok, "lav") == 0) push(c, 2);
        else if (strcmp(tok, "wink") == 0) push(c, 3);
        else if (strcmp(tok, "sink") == 0) push(c, 4);
        else if (strcmp(tok, "wist") == 0) push(c, 5);
        else if (strcmp(tok, "yebow") == 0) push(c, 6);
        else if (strcmp(tok, "geen") == 0) push(c, 7);

        else if (strcmp(tok, "print") == 0) {
            char *str = strtok(NULL, "");
            
            if (str) {
                while (*str == ' ') str++;
                
                int nl = 0;
                char *nl_at = strstr(str, " /n");

            if (nl_at) {
                    *nl_at = '\0';
                    nl = 1;
                }
                
                if (str[0] == '"') {
                    str++;
                    char *end = strrchr(str, '"');
                    if (end) *end = '\0';
                }
                
                printf("%s", str);
                if (nl) printf("\n");
                fflush(stdout);
                
                for (int i = 0; str[i]; i++) {
                    if (str[i] == '\n') {
                        c->cursor_x = 0;
                        c->cursor_y += FONT_HEIGHT + 4;
                        continue;
                    }

            		draw_char_c(c, str[i], c->cursor_x, c->cursor_y);
                    c->cursor_x += FONT_WIDTH + 2;

                    if (c->cursor_x + FONT_WIDTH >= VIDEO_WIDTH) {
                        c->cursor_x = 0;
                        c->cursor_y += FONT_HEIGHT + 4;
                    }
                }

                if (nl) {
                    c->cursor_x = 0;
                    c->cursor_y += FONT_HEIGHT + 4;
                }

                cottonwindow_update(cw, c->video, sizeof(c->video[0]) * VIDEO_WIDTH);
            }
            break;
        }
        
        else if (strcmp(tok, "wait") == 0) {
            c->ticktock = SDL_GetTicks() + pop(c);
        }

        else if (strcmp(tok, "kill") == 0) {
            exit(0);
        }

        else {
            int slot = find_var(c, tok);
            if (slot >= 0) {
                push(c, slot);
            }

            else {
                fprintf(stderr, "cot: cotton doesn't know what \"%s\" means, maybe type that out again? :<", tok);
            }
        }
        
        tok = strtok(NULL, " ");
    }
}
