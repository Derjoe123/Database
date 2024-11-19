#include "../Common/Errors.h"
#include "Parser.h"
#include <limits.h>
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
    char *input = "SELECT 123 FROM TABLE \"hello\" \'01\'";
    Parser parser = {0};
    parser_init(&parser);
    ParserSymbol symbols[] = {
        {.name = "SELECT", .index = 1},
        {.name = "FROM", .index = 2},
        {.name = "TABLE", .index = 3},
    };
    parser_register_symbols(&parser, symbols,
                            sizeof(symbols) / sizeof(ParserSymbol));
    ParsedOpCodes opCodes = parser_parse(&parser, input, strlen(input));
    printf("%zu opcodes: \n", opCodes.opCodesCount);
    for (size_t i = 0; i < opCodes.opCodesCount; i++) {
        printf("%u ", (unsigned int)opCodes.opCodes[i]);
    }
    printf("\n");
}
