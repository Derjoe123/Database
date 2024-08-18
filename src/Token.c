#include "Token.h"
#include <stdio.h>
#include <string.h>

void token_print(Token *token) {
    for (size_t i = 0; i < token->size; i++) {
        printf("%c", token->start[i]);
    }
}
Token token_create(Token_Type type, const char *start, size_t size) {
    Token token = {0};
    token.type = type;
    token.start = start;
    token.size = size;
    return token;
}
