#ifndef COTTON_H
#define COTTON_H

#include <stdint.h>

#define MEMORY_SIZE  (64 * 1024)
#define VIDEO_WIDTH  250
#define VIDEO_HEIGHT 200

typedef struct
{
    uint8_t  memory[MEMORY_SIZE];
    uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT];

    // i cant come up with a better name that gets straight to the point than "cursor" so ig it will stay this way for now,,      
    int cursor_x;
    int cursor_y;

    uint32_t ticktock;

} Cotton;

void cotton_init(Cotton *c);

#endif
