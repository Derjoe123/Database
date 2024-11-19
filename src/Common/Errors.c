#include "Errors.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
Error globalError;
void throwError(char *message, ...) {
    char buffer[512] = {0};
    va_list args;
    va_start(args, message);
    vsnprintf(buffer, 512, message, args);
    va_end(args);

    if (globalError.message == NULL) {
        globalError.message = malloc(512 * sizeof(char));
        if (globalError.message == NULL) {
            fprintf(stderr, "FATAL: Memory allocation failed\n");
            exit(1);
        }
    }

    strncpy(globalError.message, buffer, 511);
    globalError.message[511] = '\0';

    if (globalError.jmp != NULL) {
        longjmp(*globalError.jmp, 1);
    }
}
void throwSyntaxError(char *message, ...) {

    char buffer[512] = {0};
    va_list args;
    va_start(args, message);
    vsnprintf(buffer, 512, message, args);
    va_end(args);

    if (globalError.message == NULL) {
        globalError.message = malloc(512 * sizeof(char));
        if (globalError.message == NULL) {
            fprintf(stderr, "FATAL: Memory allocation failed\n");
            exit(1);
        }
    }

    strncpy(globalError.message, buffer, 511);
    globalError.message[511] = '\0';

    if (globalError.jmp != NULL) {
        longjmp(*globalError.jmp, 1);
    }
}
