#include "weave.h"
#include "cotton.h"
#include <stdio.h>

void cog_add
(Cotton *c)
{
	int b = pop(c);
    int a = pop(c);

    push(c, a + b);
}

void cog_sub
(Cotton *c)
{
    int b = pop(c);
    int a = pop(c);

    push(c, a - b);
}

void cog_mul
(Cotton *c)
{
    int b = pop(c);
    int a = pop(c);

    push(c, a * b);
}

void cog_div
(Cotton *c)
{
    int b = pop(c);
    int a = pop(c);

    // Now, i doubt anyone will really be dividing stuff by zero but you never know.., talk about le safety amirite.., heh 
    if (b == 0) {

        fprintf(stderr, "cotton: you cant divide by zero, you dummy :P\n");
        return;
    }

    push(c, a / b);
}

void cog_dotnum
(Cotton *c)
{
	printf("%d\n", pop(c));
}

void cog_dotstr
(Cotton *c)
{ 
    int ptr = pop(c);

    while (c->mem[ptr] != '\0') {

        printf("%c", c->mem[ptr]);

        ptr++;
    }

	printf("\n");
}

// while forth does use ! and @, im quirky and #notlikeothergirls so im changing ! to -> bc i think it makes more sense :P
void cog_store
(Cotton *c)
{
    int slot = pop(c);
    int num = pop(c);

    if (slot >= 0 && slot < VARS_SIZE) {

        c->vars[slot] = num;
    } 

    else {

        fprintf(stderr, "cotton: i tried to store somewhere that don't exist,, its in the void now.., :(\n");
    }
}

void cog_fetch
(Cotton *c)
{
    int slot = pop(c);

    if (slot >= 0 && slot < VARS_SIZE) {

 		push(c, c->vars[slot]);
    } 

    else {

        fprintf(stderr, "cotton: i tried to fetch from somewhere that seemingly don't exist,, erm.,.\n");
    }
}

void cog_dup
(Cotton *c)
{
    int a = pop(c);

    push(c, a);
    push(c, a);
}

void cog_drop
(Cotton *c)
{
    pop(c);
}

void cog_swap
(Cotton *c)
{
    int b = pop(c);
    int a = pop(c);

    push(c, b);
    push(c, a);
}

void cog_equal
(Cotton *c)
{
    int b = pop(c);
    int a = pop(c);

    push(c, a == b ? 1 : 0);
}

void cog_lssr
(Cotton *c)
{
    int b = pop(c);
    int a = pop(c);

    push(c, a < b ? 1 : 0);
}

void cog_grtr
(Cotton *c)
{
    int b = pop(c);
    int a = pop(c);

    push(c, a > b ? 1 : 0);
}

Cog builtins[] = {
    {"+", cog_add},
    {"-", cog_sub},
    {"*", cog_mul},
    {"/", cog_div},
    {".n", cog_dotnum},
	{".s", cog_dotstr}, 
    {"->", cog_store},
    {"@", cog_fetch},
    {"dup", cog_dup},
    {"drop", cog_drop},
    {"swap", cog_swap},
    {"=", cog_equal},
    {"<", cog_lssr},
    {">", cog_grtr},
    {NULL, NULL}
};

/*

 separator comment to split the builtins from immediates xP

*/

void cog_if
(Cotton *c)
{
    if (c->jmp_stack_p >= JMP_STACK_SIZE) {

        fprintf(stderr, "cotton: there are too many nested ifs, calm down!! D:\n");
        return;
    }

    c->mem[c->mem_len++] = OP_FJMP;
    c->jmp_stack[c->jmp_stack_p++] = c->mem_len;
    c->mem[c->mem_len++] = 0;
}

void cog_else
(Cotton *c)
{
    if (c->jmp_stack_p <= 0) {

        fprintf(stderr, "cotton: i found an else without an if,., where'd that come from??\n");
        return;
    }

    int if_target = c->jmp_stack[--c->jmp_stack_p];

    c->mem[c->mem_len++] = OP_JMP;
    c->jmp_stack[c->jmp_stack_p++] = c->mem_len;
    c->mem[c->mem_len++] = 0;

    c->mem[if_target] = c->mem_len;
}

void cog_end
(Cotton *c)
{
    if (c->jmp_stack_p <= 0) {

        fprintf(stderr, "cotton: i found an end without an if,., where'd that come from??\n");
        return;
    }

    int target = c->jmp_stack[--c->jmp_stack_p];
    c->mem[target] = c->mem_len;
}

void cog_loop
(Cotton *c)
{
	c->jmp_stack[c->jmp_stack_p++] = c->mem_len;
}

void cog_until
(Cotton *c)
{
	int loop_back = c->jmp_stack[--c->jmp_stack_p];

	c->mem[c->mem_len++] = OP_FJMP;
	c->mem[c->mem_len++] = loop_back;
}

Cog immediates [] = {
    {"if", cog_if},
    {"else", cog_else},
    {"end", cog_end},
    {"loop", cog_loop},
    {"until", cog_until},
    {NULL, NULL}
};

