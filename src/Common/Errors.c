#include "Errors.h"
#include <stdarg.h>
#include <stdio.h>
Error globalError;
void throwError(char *message, ...) {
    if (!globalError.jmp) {
        va_list args;
        va_start(args, message);
        vprintf(message, args);
        va_end(args);
        printf("\n");
    } else {
        char buffer[512] = {0};
        va_list args = {0};
        va_start(args, message);
        vsnprintf(buffer, 512, message, args);
        globalError.message = buffer;
        va_end(args);
        longjmp(*globalError.jmp, 1);
    }
}
