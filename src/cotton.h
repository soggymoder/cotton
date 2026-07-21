#ifndef COTTON_H
#define COTTON_H

#include <stdint.h>

#define MEMORY_SIZE  (64 * 1024)
#define VIDEO_WIDTH  250
#define VIDEO_HEIGHT 200

#define COTTOLETTE_SIZE 7 // would be 6 but due to high demand we invited green into the mix because green is cool <3

// i might work on better names another time xP
static const uint32_t COTTOLETTE[COTTOLETTE_SIZE] = 
{
    0xFF000000, // 0 - black
    0xFFFFFFFF, // 1 - white
    0xFF28262C, // 2 - rey
    0xFFA9A0CF, // 3 - wist
    0xFFE4D8FD, // 4 - wink
    0xFFF9F5FF, // 5 - laven
    0xFFD0F0C0, // 6 - the dude nobody invited (geen)
};

typedef struct
{
    uint8_t  memory[MEMORY_SIZE];
    uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT];

    // i cant come up with a better name that gets straight to the point than "cursor" so ig it will stay this way for now,,      
    int cursor_x;
    int cursor_y;

    uint32_t ticktock;
    uint32_t c_cottolette; //id write "current_cottolete" but that would be alot to type and im lazyyyyyyy bwaaaaaa
    
} Cotton;

void cotton_init(Cotton *c);

#endif
