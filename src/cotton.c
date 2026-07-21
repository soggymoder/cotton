#include "cotton.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>

void cotton_init(Cotton *c) 
{
    memset(c, 0, sizeof(Cotton));

    srand((unsigned int)time(NULL));

    c->c_cottolette = COTTOLETTE[1];
        
}
