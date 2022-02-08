#include <stdarg.h>
#include <stdio.h>

#include "coolcc.h"

void error(const char* message, ...) {
    fputs("ERROR: ", stderr);

    va_list args;
    va_start(args, message);
    vfprintf(stderr, message, args);
    va_end(args);
}
