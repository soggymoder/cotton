#ifndef COTTON_H
#define COTTON_H

#define OP_PUSH        0
#define OP_CALL        1
#define OP_KILL        2
#define OP_JMP         3
#define OP_FJMP        4

#define STACK_SIZE     64
#define JMP_STACK_SIZE 32
#define MEM_SIZE       1024
#define VARS_SIZE      64

typedef struct Cotton Cotton;

typedef struct {
    char *name;
    void (*code)(Cotton *c);
} Word;

struct Cotton {
    int stack[STACK_SIZE];
    int stack_p;

    int jmp_stack[JMP_STACK_SIZE];
    int jmp_stack_p; 

    int mem[MEM_SIZE];
    int instruct_p;
    int mem_len;

    int vars[VARS_SIZE];
};

void cotton_init
(Cotton *c);

void push
(Cotton *c, int v);

int pop
(Cotton *c);

#endif
