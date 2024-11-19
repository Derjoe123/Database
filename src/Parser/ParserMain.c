#include "../Common/Errors.h"
#include "Lexer.h"
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
jmp_buf env;
int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    // Set up longjump for error handling
    bool error = false;
    setjmp(env);
    globalError.jmp = &env;
    if (error) {
        printf("Error: %s\n", globalError.message);
        return 1;
    }
    error = true;

    char *input = "SELECT * FROM table";
    Lexer lexer = {0};
    lexer_init(&lexer, input, strlen(input));
    while (!lexer_eof(&lexer)) {
        Token token = lexer_next(&lexer);
        if (token.type == TOKEN_ERROR) {
            lexer_print_error(&lexer, &token);
            throwError("Invalid Token");
        }
        token_print(&token);
        printf("\n");
    }
}
