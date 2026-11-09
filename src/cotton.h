#ifndef COTTON_H
#define COTTON_H

#include <stdbool.h>

#define OP_PUSH_INT	   0
#define OP_PUSH_STR    1
#define OP_CALL        2
#define OP_KILL        3
#define OP_JMP         4
#define OP_FJMP        5

#define STACK_SIZE      256
#define CTRL_STACK_SIZE 64
#define MEM_SIZE        4096
#define INT_VARS_SIZE   256
#define STR_VARS_SIZE   256

typedef struct Cotton Cotton;

typedef struct {
    
    char *name;
    void (*code)(Cotton *c);

} Cog;

struct Cotton {

	int stack[STACK_SIZE];
    int stack_ptr;

    int ctrl_stack[CTRL_STACK_SIZE];
    int ctrl_stack_ptr; 

    int mem[MEM_SIZE];
    int instruction_ptr;
    int mem_len;

    int  int_vars[INT_VARS_SIZE];
	bool int_vars_assigned[INT_VARS_SIZE];

	int str_vars[STR_VARS_SIZE];
	bool str_vars_assigned[STR_VARS_SIZE];
};

void cotton_init
(Cotton *c);

void push
(Cotton *c, int v);

int pop
(Cotton *c);

#endif
