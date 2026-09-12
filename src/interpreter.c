/* I eventually need to split this file before it gets too big, i'll keep this comment here as a mental reminder,.,
ooooo get off your ass dani ooooo go work ooooo dani you're a bum oooooo oooooo you suckkk oooooooo,., sorry me.., */

#include "cotton.h"
#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void word_add
(Cotton *c)
{
	int b = pop(c);
    int a = pop(c);

    push(c, a + b);
}

static void word_sub
(Cotton *c)
{
    int b = pop(c);
    int a = pop(c);

    push(c, a - b);
}

static void word_mul
(Cotton *c)
{
    int b = pop(c);
    int a = pop(c);

    push(c, a * b);
}

static void word_div
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

static void word_dotnum
(Cotton *c)
{
	printf("%d\n", pop(c));
}

static void word_dotstr
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
static void word_store
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

static void word_fetch
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

static void word_dup
(Cotton *c)
{
    int a = pop(c);

    push(c, a);
    push(c, a);
}

static void word_drop
(Cotton *c)
{
    pop(c);
}

static void word_swap
(Cotton *c)
{
    int b = pop(c);
    int a = pop(c);

    push(c, b);
    push(c, a);
}

static void word_equal
(Cotton *c)
{
    int b = pop(c);
    int a = pop(c);

    push(c, a == b ? 1 : 0);
}

static void word_lssr
(Cotton *c)
{
    int b = pop(c);
    int a = pop(c);

    push(c, a < b ? 1 : 0);
}

static void word_grtr
(Cotton *c)
{
    int b = pop(c);
    int a = pop(c);

    push(c, a > b ? 1 : 0);
}

static Word builtins[] = {
    {"+", word_add},
    {"-", word_sub},
    {"*", word_mul},
    {"/", word_div},
    {".n", word_dotnum},
	{".s", word_dotstr}, 
    {"->", word_store},
    {"@", word_fetch},
    {"dup", word_dup},
    {"drop", word_drop},
    {"swap", word_swap},
    {"=", word_equal},
    {"<", word_lssr},
    {">", word_grtr},
    {NULL, NULL}
};

/* this comment serves as a separator to split the words above from the words below, the words below run straight away while compiling
instead of waiting to be ran later, forth calls these "immediate words" so thats what im calling them too :P (if it aint broke dont fix it as they say) */

static void word_if
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

static void word_else
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

static void word_end
(Cotton *c)
{
    if (c->jmp_stack_p <= 0) {

        fprintf(stderr, "cotton: i found an end without an if,., where'd that come from??\n");
        return;
    }

    int target = c->jmp_stack[--c->jmp_stack_p];
    c->mem[target] = c->mem_len;
}

static void word_loop
(Cotton *c)
{
	c->jmp_stack[c->jmp_stack_p++] = c->mem_len;
}

static void word_until
(Cotton *c)
{
	int loop_back = c->jmp_stack[--c->jmp_stack_p];

	c->mem[c->mem_len++] = OP_FJMP;
	c->mem[c->mem_len++] = loop_back;
}

static Word immediates[] = {
    {"if", word_if},
    {"else", word_else},
    {"end", word_end},
    {"loop", word_loop},
    {"until", word_until},
    {NULL, NULL}
};

/* Another comment just so i can separate the immediates from the compiler and runtime functions blehhhhhh
words words words words more words SO many words OH MY GOD THERE ARE SO MANY WORDS WORDS WORDS WORDS AAAAAA*/

// i feel like the compiler's a bit messy atm but i suppose this is yet another thing i shall revise when i eventually split the interpreter :P
void cotton_compile
(Cotton *c, char *line)
{
    line[strcspn(line, "\r\n")] = 0;
    if (line[0] == '\0' || (line[0] == '/' && line[1] == '/')) return;

// this part of the compiling process is just for cotton to find strings 

    int i = 0;

    while (line[i] != '\0') {

        if (c->mem_len >= MEM_SIZE - 2) {

        	fprintf(stderr, "cotton: i have ran out of memory,., can't compile anymore :<\n");
            return;
        }

        if (line[i] == ' ') {

            i++;
        }

        else if (line[i] == '"') {
            
            int string_start = i + 1;

            for (i = string_start; line[i] != '\0'; i++) {

                if (line[i] == '"') {

                    line[i] = '\0';

                    break;
                }

                else if (line[i] == '\0') {

                    fprintf(stderr, "cotton: you forgot to close a string.,. i cant compile the file until you fix it :P");
                    return;
                }

            }

            c->mem[c->mem_len++] = OP_PUSH_STR;

            for (int j = string_start; line[j] != '\0'; j++) {

                c->mem[c->mem_len++] = line[j];
            }

			c->mem[c->mem_len++] = '\0';
                        
            i++;
        }


// now it resumes to it's usual "reading words character by character" behaviour :P

        else {
            
            int tok_start = i;
            
            while (line[i] != ' ' && line[i] != '\0') {

                i++;
            }
            
            // Here i just terminate the token temporarily
            char temp = line[i];
            line[i] = '\0';
            
            char *tok = line + tok_start;

            /* This is subjective to change but i thought that using ? for conditionals so stuff reads out as
            "is this happening? if so, then, blah blah blah" was a good idea, atleast sounded good in my head :P*/

            if (strcmp(tok, "?") == 0) {
            // oh yeah, also, this doesn't really do anything, its just here to make if statements look nicer x3 
            }

            else {            

                int found_immediate = 0;

                for (int j = 0; immediates[j].name != NULL; j++) {

                    if (strcmp(tok, immediates[j].name) == 0) {

                        immediates[j].code(c);
                        found_immediate = 1;
                        break;
                    }
                }

                if (!found_immediate) {
                    
                    int is_num = 1;
                    
                    for (int j = 0; tok[j]; j++) {
                        
                        if (!isdigit(tok[j]) && !(j == 0 && tok[j] == '-' && tok[1] != '\0')) {

                            is_num = 0;
                            break;
                        }
                    }

                    if (is_num) {

                        c->mem[c->mem_len++] = OP_PUSH;
                        c->mem[c->mem_len++] = atoi(tok);
                    } 

                    else {
                        
                        int found = 0;

                        for (int j = 0; builtins[j].name != NULL; j++) {
                            
                            if (strcmp(tok, builtins[j].name) == 0) {

                                c->mem[c->mem_len++] = OP_CALL;
                                c->mem[c->mem_len++] = j;
                                found = 1;
                                break;
                            }
                        }

                        if (!found) {

                            fprintf(stderr, "cotton: sorry, i don't know what \"%s\" means :(\n", tok);
                        }
                    }
                }
            }

			// now i just add this back so that the outer loop doesn't get kirkified broken heart emoji 
            line[i] = temp;
        }
    }
}

void cotton_run
(Cotton *c)
{
    c->instruct_p = 0;

    while (c->instruct_p < c->mem_len) {
        
        int op = c->mem[c->instruct_p++];

        switch (op) {

            case OP_PUSH:
                push(c, c->mem[c->instruct_p++]);

                break;

            case OP_CALL: {
                int id = c->mem[c->instruct_p++];
                builtins[id].code(c);

                break;
            }

            case OP_JMP: {
                c->instruct_p = c->mem[c->instruct_p];

                break;
            }

            case OP_FJMP: {

                int target = c->mem[c->instruct_p++];

                if (pop(c) == 0) {
                    c->instruct_p = target;
                }

                break;
            }

            case OP_PUSH_STR:

             	push(c, c->instruct_p);

                while (c->mem[c->instruct_p] != '\0') {
                    c->instruct_p++;
                }
                
                c->instruct_p++;

                break;

            case OP_KILL:
                
                return;

        }
    }
}
