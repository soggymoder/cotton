#include <stdio.h>
#include "cotton.h"
#include "cottonwindow.h"
#include "interpreter.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: cotton <file.cot>\n");
        return 1;
    }

    CottonWindow cw;

    if (!cottonwindow_init(&cw, "cotton", VIDEO_WIDTH * 4, VIDEO_HEIGHT * 4, VIDEO_WIDTH, VIDEO_HEIGHT))
        return 1;

    Cotton cotton;
    cotton_init(&cotton);

    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        fprintf(stderr, "cotton couldn't open \"%s\" :(\n", argv[1]);
        return 1;
    }

    int pitch = sizeof(cotton.video[0]) * VIDEO_WIDTH;
    int quit = 0;

    while (!quit)
    {
        quit = cottonwindow_process_input(&cw, NULL);

        if (SDL_GetTicks() >= cotton.ticktock)
        {
            cotton_interpret(&cotton, &cw, file);
        }

        cottonwindow_update(&cw, cotton.video, pitch);
    }

    fclose(file);
    cottonwindow_destroy(&cw);
    return 0;
}
