#include "cotton.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void cotton_init
(Cotton *c)
{
    memset(c, 0, sizeof(Cotton));

    srand((unsigned int)time(NULL));

	c->stack_p = 0;

    c->c_cottolette = COTTOLETTE[1];
}
