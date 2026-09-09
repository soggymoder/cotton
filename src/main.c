#include "cotton.h"
#include "interpreter.h"
#include <stdio.h>

int main
(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "cotton: cotton couldn't find any .cot files to run :P\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");

    if (!f) {
        fprintf(stderr, "cotton: couldn't open \"%s\", is this even a file.,.? :<\n", argv[1]);
        return 1;
    }

    Cotton c;
    cotton_init(&c);

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        cotton_compile(&c, line);
    }

    fclose(f);

    c.mem[c.mem_len++] = OP_KILL;

    cotton_run(&c);

    return 0;
}
