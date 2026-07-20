#include "cottonwindow.h"
#include <stdio.h>

int cottonwindow_init(CottonWindow *cw, const char *title, int window_w, int window_h, int texture_w, int texture_h)
{
    SDL_Init(SDL_INIT_VIDEO);

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "cotton couldn't start SDL :( : %s\n", SDL_GetError());
        return 0;
    }

    cw->window = SDL_CreateWindow(title, 0, 0, window_w, window_h, SDL_WINDOW_SHOWN);
    if (!cw->window)
    {
        fprintf(stderr, "cotton couldn't create a window :( : %s\n", SDL_GetError());
        return 0;
    }

    cw->renderer = SDL_CreateRenderer(cw->window, -1, 0);
    if (!cw->renderer)
    {
        fprintf(stderr, "cotton couldn't render this :( : %s\n", SDL_GetError());
        return 0;
    }

    cw->texture = SDL_CreateTexture(cw->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texture_w, texture_h);
    if (!cw->texture)
    {
        fprintf(stderr, "cotton couldn't apply textures :( : %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

void cottonwindow_update(CottonWindow *cw, const void *buffer, int pitch)
{
    SDL_UpdateTexture(cw->texture, NULL, buffer, pitch);
    SDL_RenderClear(cw->renderer);
    SDL_RenderCopy(cw->renderer, cw->texture, NULL, NULL);
    SDL_RenderPresent(cw->renderer);
}

int cottonwindow_process_input(CottonWindow *cw, uint8_t *keys)
{
    (void)cw;
    (void)keys;
    int quit = 0;
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                quit = 1;
                break;
            }

            case SDL_KEYDOWN:
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    quit = 1;
                break;
            }
        }
    }

    return quit;
}

void cottonwindow_destroy(CottonWindow *cw)
{
    SDL_DestroyTexture(cw->texture);
    SDL_DestroyRenderer(cw->renderer);
    SDL_DestroyWindow(cw->window);
    SDL_Quit();
}
