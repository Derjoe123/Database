#pragma once
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
typedef struct {
    char *message;
    jmp_buf *jmp;
} Error;
extern Error globalError;
void throwError(char *message, ...);
void throwSyntaxError(char *message, ...);
#define UNREACHABLE                                                            \
    throwError("Unreachable code reached at %s: %d", __FILE__, __LINE__);
