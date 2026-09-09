#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "cotton.h"

void cotton_compile
(Cotton *c, char *line);

void cotton_run
(Cotton *c);

#endif
