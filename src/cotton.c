#include "cotton.h"
#include <stdio.h>

void cotton_init
(Cotton *c) 
{
    c->stack_p = 0;
    c->jmp_stack_p = 0;
    c->instruct_p = 0;
    c->mem_len = 0;

    for (int i = 0; i < VARS_SIZE; i++) {
        c->vars[i] = 0;
    }
}

void push
(Cotton *c, int v) 
{
    if (c->stack_p >= STACK_SIZE) {
        fprintf(stderr, "cotton: stack overflow,., like the website,.., :<\n");
        return;
    }

    c->stack[c->stack_p++] = v;
}

int pop
(Cotton *c) 
{
    if (c->stack_p <= 0) {
        fprintf(stderr, "cotton: stack underflow :<\n");
        return 0;
    }

    return c->stack[--c->stack_p];
}
