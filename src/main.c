#include "cotton.h"
#include "cottonwindow.h"
#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main
(int argc, char **argv)
{

    // added this specifically with the new user input command in mind, makes it so the terminal isnt so cluttered for when there is user input needed, atleast for me :P
    printf("\033[2J\033[H");

    // thank you chld for the idea of changing the scale and having it be optional ^^
    int scale = 4;
    char *filename = NULL;

    if (argc == 4 && strcmp(argv[1], "-s") == 0) {
        scale = atoi(argv[2]);
        filename = argv[3];
    } else if (argc == 2) {
        filename = argv[1];
    } else {
        fprintf(stderr, "usage: cotton <-s scale (scale being 2 or 4)> <file.cot>\n");
        return 1;
    }

    CottonWindow cw;

    if (!cottonwindow_init(&cw, "cotton", VIDEO_WIDTH * scale, VIDEO_HEIGHT * scale, VIDEO_WIDTH, VIDEO_HEIGHT)) {
        return 1;
    }

    Cotton cotton;
    cotton_init(&cotton);

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "cotton couldn't open \"%s\" :(\n", filename);
        return 1;
    }

    int pitch = sizeof(cotton.video[0]) * VIDEO_WIDTH;
    int quit = 0;

    while (!quit) {
        quit = cottonwindow_process_input(&cw, NULL);

        if (SDL_GetTicks() >= cotton.ticktock) {
            cotton_interpret(&cotton, &cw, file);
        }

        cottonwindow_update(&cw, cotton.video, pitch);
    }

    fclose(file);
    cottonwindow_destroy(&cw);
    return 0;
}
