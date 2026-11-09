#include "cotton.h"
#include <stdio.h>

void cotton_init
(Cotton *c) 
{
    c->stack_ptr = 0;
    c->ctrl_stack_ptr = 0;
    c->instruction_ptr = 0;
    c->mem_len = 0;

    for (int i = 0; i < INT_VARS_SIZE; i++) {
        
        c->int_vars[i] = 0;
		c->int_vars_assigned[i] = false;
    }
}

void push
(Cotton *c, int v) 
{
    if (c->stack_ptr >= STACK_SIZE) {
        
        fprintf(stderr, "cotton: stack overflow,., like the website,.., :<\n");
        return;
    }

    c->stack[c->stack_ptr++] = v;
}

int pop
(Cotton *c) 
{
    if (c->stack_ptr <= 0) {

        fprintf(stderr, "cotton: stack underflow :<\n"); 
        return 0;
    }

    return c->stack[--c->stack_ptr];
}
