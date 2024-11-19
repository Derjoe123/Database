#pragma once
#include <stddef.h>
typedef enum {
    TOKEN_ERROR = 0,
    TOKEN_EOF,

    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,

    TOKEN_HASH,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_BANG,
    TOKEN_BANG_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_DOT,
    TOKEN_COLON
} Token_Type;
typedef struct {
    Token_Type type;
    const char *start;
    size_t size;
} Token;
void token_print(Token *token);
Token token_create(Token_Type type, const char *start, size_t size);
