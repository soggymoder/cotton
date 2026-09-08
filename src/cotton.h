#ifndef COTTON_H
#define COTTON_H

#include <stdint.h>

#define MEMORY_SIZE (64 * 1024)
#define VIDEO_WIDTH 250
#define VIDEO_HEIGHT 200

#define COTTOLETTE_SIZE  8

#define VARS 32 // i think this is enough,, not like cot programs can be that complex rn anyway :P
#define VAR_NAME_LEN 16
#define VAR_VAL_LEN 32

static const uint32_t 
COTTOLETTE[COTTOLETTE_SIZE] = {
    0xFF1C1C1C, // 0 - blabla
    0xFFFAFAFA, // 1 - whie
    0xFFECE3FF, // 2 - lav
    0xFFE4D8FD, // 3 - wink
    0xFFD4C5F5, // 4 - sink
    0xFFA9A0CF, // 5 - wist
    0xFFFFFBDA, // 6 - yebow (random kid)
    0xFFD0F0C0  // 7 - geen (the (first) dude nobody invited)
};

typedef struct
{
    char name[VAR_NAME_LEN];
    char value[VAR_VAL_LEN];
} CotVar;

typedef struct
{
    uint8_t memory[MEMORY_SIZE];
    uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT];

    // i cant come up with a better name that gets straight to the point than
    // "cursor" so ig it will stay this way for now,,
    int cursor_x;
    int cursor_y;

    uint32_t ticktock;
    uint32_t c_cottolette; // id write "current_cottolete" but that would be
                           // alot to type and im lazyyyyyyy bwaaaaaa

    CotVar vars[VARS];
	int var_count;

	int stack[32];
	int stack_p;

} Cotton;

void cotton_init
(Cotton *c);
void cotton_store_var
(Cotton *cotton, const char *name, const char *value);

#endif

