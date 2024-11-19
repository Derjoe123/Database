#include "Token.h"
#include <stdio.h>

void token_print(Token *token) {
    printf("%.*s", (int)token->size, token->start);
}
Token token_create(Token_Type type, const char *start, size_t size) {
    Token token = {0};
    token.type = type;
    token.start = start;
    token.size = size;
    return token;
}
