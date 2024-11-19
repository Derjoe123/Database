#include "Lexer.h"
#include "../Common/Errors.h"
#include "Token.h"
#include <ctype.h>
#include <stdio.h>
bool valid_identifier_char(char chr) {
    switch (chr) {
    case '_':
    case '$':
        return true;
    default:
        return isdigit(chr) || isalpha(chr);
    }
    UNREACHABLE
    return false;
}
Token lexer_next(Lexer *lexer) {
    if (!lexer->Initialized) {
        throwError("Lexer not initialized, call lexer_init first");
    }
    if (lexer_eof(lexer)) {
        return token_create(TOKEN_EOF, "", 0);
    }
    if ((isdigit(*lexer->current) || *lexer->current == '-') &&
        !lexer_eof(lexer)) {
        Token token = token_create(TOKEN_INTEGER, lexer->current, 1);
        const char *tokenStart = lexer->current;
        bool encounteredDot = false;
        lexer->current++;
        while (isdigit(*lexer->current) || *lexer->current == '.') {
            if (*lexer->current == '.') {
                if (encounteredDot) {
                    throwSyntaxError("%.*s, more than 1 decimal point",
                                     lexer->current - tokenStart, tokenStart);
                    break;
                }
                encounteredDot = true;
                token = token_create(TOKEN_FLOAT, tokenStart, 1);
            }
            lexer->current++;
        }
        token.size = lexer->current - token.start;
        return token;
    }
    if (isalpha(*lexer->current)) {
        Token token = token_create(TOKEN_IDENTIFIER, lexer->current, 1);
        while (!lexer_eof(lexer) && valid_identifier_char(*lexer->current)) {
            lexer->current++;
        }
        token.size = lexer->current - token.start;
        return token;
    }
    switch (*lexer->current) {
    case '+':
        lexer->current++;
        return token_create(TOKEN_PLUS, lexer->current - 1, 1);
    case '-':
        lexer->current++;
        return token_create(TOKEN_MINUS, lexer->current - 1, 1);
    case '*':
        lexer->current++;
        return token_create(TOKEN_STAR, lexer->current - 1, 1);
    case '%':
        lexer->current++;
        return token_create(TOKEN_PERCENT, lexer->current - 1, 1);
    case '/':
        lexer->current++;
        return token_create(TOKEN_SLASH, lexer->current - 1, 1);
    case '(':
        lexer->current++;
        return token_create(TOKEN_LEFT_PAREN, lexer->current - 1, 1);
    case ')':
        lexer->current++;
        return token_create(TOKEN_RIGHT_PAREN, lexer->current - 1, 1);
    case ' ':
    case '\t':
    case '\n':
        lexer->current++;
        return lexer_next(lexer);
    case '\"': {
        lexer->current++;
        Token token = token_create(TOKEN_STRING, lexer->current, 0);
        while (*lexer->current != '\"' && !lexer_eof(lexer)) {
            lexer->current++;
        }
        token.size = lexer->current - token.start;
        if (*lexer->current == '\"') {
            lexer->current++;
            return token;
        }
        break;
    }
    case '\'': {
        lexer->current++;
        Token token = token_create(TOKEN_RAW_DATA, lexer->current, 0);
        while (*lexer->current != '\'' && !lexer_eof(lexer)) {
            lexer->current++;
        }
        token.size = lexer->current - token.start;
        if (*lexer->current == '\'') {
            lexer->current++;
            return token;
        }
        break;
    }
    case '.':
        lexer->current++;
        return token_create(TOKEN_DOT, lexer->current - 1, 1);
    case ',':
        lexer->current++;
        return token_create(TOKEN_COMMA, lexer->current - 1, 1);
    case ';':
        lexer->current++;
        return token_create(TOKEN_SEMICOLON, lexer->current - 1, 1);
    case ':':
        lexer->current++;
        return token_create(TOKEN_COLON, lexer->current - 1, 1);
    case '=':
        lexer->current++;
        if (*lexer->current == '=') {
            lexer->current++;
            return token_create(TOKEN_EQUAL_EQUAL, lexer->current - 2, 2);
        }
        return token_create(TOKEN_EQUAL, lexer->current - 1, 1);
    case '!':
        lexer->current++;
        if (*lexer->current == '=') {
            lexer->current++;
            return token_create(TOKEN_BANG_EQUAL, lexer->current - 2, 2);
        }
        return token_create(TOKEN_BANG, lexer->current - 1, 1);
    case '>':
        lexer->current++;
        if (*lexer->current == '=') {
            lexer->current++;
            return token_create(TOKEN_GREATER_EQUAL, lexer->current - 2, 2);
        }
        return token_create(TOKEN_GREATER, lexer->current - 1, 1);
    case '<':
        lexer->current++;
        if (*lexer->current == '=') {
            lexer->current++;
            return token_create(TOKEN_LESS_EQUAL, lexer->current - 2, 2);
        }
        return token_create(TOKEN_LESS, lexer->current - 1, 1);
    case '&':
        lexer->current++;
        if (*lexer->current == '&') {
            lexer->current++;
            return token_create(TOKEN_AND, lexer->current - 2, 2);
        }
        return token_create(TOKEN_ERROR, lexer->current - 1, 1);
    case '|':
        lexer->current++;
        if (*lexer->current == '|') {
            lexer->current++;
            return token_create(TOKEN_OR, lexer->current - 2, 2);
        }
        return token_create(TOKEN_ERROR, lexer->current - 1, 1);
    case '{':
        lexer->current++;
        return token_create(TOKEN_LEFT_BRACE, lexer->current - 1, 1);
    case '}':
        lexer->current++;
        return token_create(TOKEN_RIGHT_BRACE, lexer->current - 1, 1);
    case '[':
        lexer->current++;
        return token_create(TOKEN_LEFT_BRACKET, lexer->current - 1, 1);
    case ']':
        lexer->current++;
        return token_create(TOKEN_RIGHT_BRACKET, lexer->current - 1, 1);
    case '#':
        lexer->current++;
        return token_create(TOKEN_HASH, lexer->current - 1, 1);
    default:
        return token_create(TOKEN_ERROR, lexer->current, 1);
    }
    UNREACHABLE
    return token_create(TOKEN_ERROR, lexer->current, 1);
}
void lexer_init(Lexer *lexer, const char *source, size_t size) {
    lexer->start = source;
    lexer->size = size;
    lexer->current = source;
    lexer->Initialized = true;
}

bool lexer_eof(Lexer *lexer) {
    return lexer->current >= lexer->start + lexer->size;
}

void lexer_print_error(Lexer *lexer, Token *errorToken) {
    printf("Invalid Token: ");
    for (size_t i = 0; i < errorToken->size; i++) {
        printf("%c", errorToken->start[i]);
    }
    printf("\n \tat: %d bytes", (int)(errorToken->start - lexer->start));
    printf("\n");
}
