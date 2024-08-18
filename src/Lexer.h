#pragma once
#include "Token.h"
#include <stdbool.h>
#include <stddef.h>
typedef struct {
    bool Initialized;
    const char *start;
    size_t size;
    const char *current;
} Lexer;
Token lexer_next(Lexer *lexer);
void lexer_init(Lexer *lexer, const char *source, size_t size);
bool lexer_eof(Lexer *lexer);
void lexer_print_error(Lexer *lexer, Token *errorToken);
