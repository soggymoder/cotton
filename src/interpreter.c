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

// Now i doubt anyone will really be dividing stuff by zero but you never know.., talk about le safety amirite.., heh 
    if (b == 0) {
        fprintf(stderr, "cotton: you cant divide by zero, you dummy :P\n");
        return;
    }

    push(c, a / b);
}

static void word_dot
(Cotton *c)
{
    printf("%d\n", pop(c));
}

// while forth does use ! and @, im quirky and #notlikeothergirls so im changing ! to -> bc i think it makes more sense :P
static void word_store
(Cotton *c)
{
    int slot = pop(c);
    int num = pop(c);

    if (slot >= 0 && slot < VARS_SIZE) {
        c->vars[slot] = num;
    } else {
        fprintf(stderr, "cotton: tried to store somewhere that don't exist.,. its in the void now.., :<\n");
    }
}

static void word_fetch
(Cotton *c)
{
    int slot = pop(c);

    if (slot >= 0 && slot < VARS_SIZE) {
        push(c, c->vars[slot]);
    } else {
        fprintf(stderr, "cotton: tried to fetch from somewhere that don't exist,., :<\n");
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

// should be enough for tonight.,., i shall be borrowing more from forth, forth is awesome, i love you forth, i forth <3 :3
static Word builtins[] = {
    {"+", word_add},
    {"-", word_sub},
    {"*", word_mul},
    {"/", word_div},
    {".", word_dot},
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

void cotton_compile
(Cotton *c, char *line)
{
    line[strcspn(line, "\r\n")] = 0;
    if (line[0] == '\0' || (line[0] == '/' && line[1] == '/')) return;

    char *tok = strtok(line, " ");

    while (tok) {
        if (c->mem_len >= MEM_SIZE - 2) {
            fprintf(stderr, "cotton: memory is full,., can't compile anymore :<\n");
            return;
        }

        int is_num = 1;
        for (int i = 0; tok[i]; i++) {
            if (!isdigit(tok[i]) && !(i == 0 && tok[i] == '-')) {
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
            for (int i = 0; builtins[i].name != NULL; i++) {
                if (strcmp(tok, builtins[i].name) == 0) {
                    c->mem[c->mem_len++] = OP_CALL;
                    c->mem[c->mem_len++] = i;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                fprintf(stderr, "cotton: cotton doesn't know what \"%s\" means,., :<\n", tok);
            }
        }
        tok = strtok(NULL, " ");
    }
}

void cotton_run
(Cotton *c)
{
    c->instruct_p = 0;

    while (c->instruct_p < c->mem_len) {
        int op = c->mem[c->instruct_p++];

        if (op == OP_PUSH) {
            push(c, c->mem[c->instruct_p++]);
        }

        else if (op == OP_CALL) {
            int idx = c->mem[c->instruct_p++];
            builtins[idx].code(c);
        }

        else if (op == OP_KILL) {
            break;
        }
    }
}
