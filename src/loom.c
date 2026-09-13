/* I eventually need to split this file before it gets too big, i'll keep this comment here as a mental reminder,.,
ooooo get off your ass dani ooooo go work ooooo dani you're a bum oooooo oooooo you suckkk oooooooo,., sorry me.., */

// FUTURE DANI HERE !!! I DID IT !!! I GOT OFF MY LAZY ASS !!! RAGHHHHHHHHHHHHHH

#include "cotton.h"
#include "loom.h"
#include "weave.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
