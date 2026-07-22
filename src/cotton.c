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

    c->c_cottolette = COTTOLETTE[1];
}

void cotton_store_var
(Cotton *cotton, const char *name, const char *value)
{
    for (int i = 0; i < cotton->var_count; i++) {
        if (strcmp(cotton->vars[i].name, name) == 0) {
            strncpy(cotton->vars[i].value, value, VAR_VAL_LEN - 1);
            cotton->vars[i].value[VAR_VAL_LEN - 1] = '\0';
            return;
        }
    }

    if (cotton->var_count < VARS) {
        strncpy(cotton->vars[cotton->var_count].name, name, VAR_NAME_LEN - 1);
        cotton->vars[cotton->var_count].name[VAR_NAME_LEN - 1] = '\0';

        strncpy(cotton->vars[cotton->var_count].value, value, VAR_VAL_LEN - 1);
        cotton->vars[cotton->var_count].value[VAR_VAL_LEN - 1] = '\0';

        cotton->var_count++;
    } else {
        fprintf(stderr,
                "cotton ran out of variable slots :( - max ammount is %d\n",
                VARS);
    }
}
