#ifndef COTTONWINDOW_H
#define COTTONWINDOW_H

#include <SDL2/SDL.h>
#include <stdint.h>

typedef struct
{
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Texture  *texture;

} CottonWindow;

int  cottonwindow_init(CottonWindow *cw, const char *title, int window_w, int window_h, int texture_w, int texture_h);
void cottonwindow_update(CottonWindow *cw, const void *buffer, int pitch);
int  cottonwindow_process_input(CottonWindow *cw, uint8_t *keys);
void cottonwindow_destroy(CottonWindow *cw);

#endif
