#include "Parser.h"
#include "Lexer.h"
#include "Token.h"
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "../Common/Settings.h"

void parser_init(Parser *parser) {
    parser->opCodes = malloc(sizeof(PARSER_OPCODE_TYPE) * DEFAULT_OPCODE_COUNT);
    parser->opCodesMaxSize = DEFAULT_OPCODE_COUNT;
    parser->opCodeSize = 0;
    parser->dataPrimitivesTable.count = 0;
    parser->dataPrimitivesTable.dataPrimitives =
        malloc(sizeof(DataPrimitive) * PARSER_DEFAULT_DATAPRIMITIVES_COUNT);
    parser->symbolTree = NULL;
}
void parser_destroy(Parser *parser) {
    free(parser->opCodes);
    free(parser->dataPrimitivesTable.dataPrimitives);
}

void parser_register_symbols(Parser *parser, ParserSymbol *symbols,
                             size_t count) {
    if (!parser->symbolTree) {
        parser->symbolTree = parser_symbol_tree_create(symbols, count);
        return;
    }
    for (size_t i = 0; i < count; i++) {
        if (!symbols[i].name) {
            throwError("tried adding empty symbol name to symbol tree");
        }
        parser_symbol_tree_insert(parser->symbolTree, &symbols[i]);
    }
}
enum DataPrimitiveType get_integer_size(long long number) {
    number = llabs(number);
    if (number < SCHAR_MAX) {
        return PRIM_INT8;
    }
    if (number < SHRT_MAX) {
        return PRIM_INT16;
    }
    if (number < INT_MAX) {
        return PRIM_INT32;
    }
    if (number < LONG_MAX) {
        return PRIM_INT64;
    }
    UNREACHABLE;
    return PRIM_NONE;
}
PARSER_OPCODE_TYPE parser_parse_token(Parser *parser, Token *token) {
    switch (token->type) {
    case TOKEN_IDENTIFIER: {
        ParserSymbol *symbol = parser_symbol_tree_get_symbol(
            parser->symbolTree, token->start, token->size);
        if (symbol) {
            return symbol->index;
        }
    } break;
    case TOKEN_INTEGER: {
        char *integer = malloc(token->size + 1);
        strncpy(integer, token->start, token->size);
        integer[token->size + 1] = '\0';
        char *endptr = NULL;
        long long num = 0;
        num = strtoll(integer, &endptr, 10);
        if (endptr == integer) {
            throwError("No digit found in: %s", integer);
        } else if (*endptr != '\0') {
            if (isdigit(*endptr))
                throwError("Input number: %s too large, largest possible "
                           "number is 2^63 - 1",
                           integer);
            throwError("Invalid int character: %c in: %s", *endptr, integer);
        }
        switch (get_integer_size(num)) {
        case PRIM_INT8: {
            dataprimitivestable_add(
                &parser->dataPrimitivesTable,
                (DataPrimitive){.type = PRIM_INT8, .data.int8 = num});
        } break;
        case PRIM_INT16: {
            dataprimitivestable_add(
                &parser->dataPrimitivesTable,
                (DataPrimitive){.type = PRIM_INT16, .data.int16 = num});
        } break;
        case PRIM_INT32: {
            dataprimitivestable_add(
                &parser->dataPrimitivesTable,
                (DataPrimitive){.type = PRIM_INT32, .data.int32 = num});
        } break;
        case PRIM_INT64: {
            dataprimitivestable_add(
                &parser->dataPrimitivesTable,
                (DataPrimitive){.type = PRIM_INT64, .data.int64 = num});
        } break;
        default:
            UNREACHABLE;
            break;
        }
        return parser->dataPrimitivesTable.count - 1 +
               OPCODE_DATAPRIMITIVES_BEGIN;
    } break;
    case TOKEN_FLOAT: {
        char *floatString = malloc(token->size + 1);
        strncpy(floatString, token->start, token->size);
        floatString[token->size + 1] = '\0';
        char *endptr = NULL;
        double num = 0.0;
        num = strtod(floatString, &endptr);
        if (endptr == floatString) {
            throwError("No digit found in: %s", floatString);
        } else if (*endptr != '\0') {
            throwError("Invalid float character: %c in: %s", *endptr,
                       floatString);
        }
        if (num < FLT_MAX) {
            dataprimitivestable_add(
                &parser->dataPrimitivesTable,
                (DataPrimitive){.type = PRIM_FLOAT32,
                                .data.float32 = (float)num});
        } else {
            dataprimitivestable_add(
                &parser->dataPrimitivesTable,
                (DataPrimitive){.type = PRIM_FLOAT64, .data.float64 = num});
        }
        return parser->dataPrimitivesTable.count - 1 +
               OPCODE_DATAPRIMITIVES_BEGIN;
    } break;
    case TOKEN_STRING: {
        char *newStr = malloc(token->size + 1);
        strncpy(newStr, token->start, token->size);
        newStr[token->size + 1] = '\0';
        dataprimitivestable_add(
            &parser->dataPrimitivesTable,
            (DataPrimitive){.type = PRIM_STRING, .data.string = newStr});
        return parser->dataPrimitivesTable.count - 1 +
               OPCODE_DATAPRIMITIVES_BEGIN;
    } break;
    case TOKEN_RAW_DATA: {
        char *newData = malloc(token->size);
        memcpy(newData, token->start, token->size);
        RawData *raw = malloc(sizeof(RawData));
        raw->size = token->size;
        raw->data = newData;
        dataprimitivestable_add(
            &parser->dataPrimitivesTable,
            (DataPrimitive){.type = PRIM_RAW, .data.raw = raw});
        return parser->dataPrimitivesTable.count - 1 +
               OPCODE_DATAPRIMITIVES_BEGIN;
    } break;
    default:
        return 0;
    }
    return 0;
}
ParsedOpCodes parser_parse(Parser *parser, const char *data, size_t length) {
    (void)parser;
    ParsedOpCodes codes = {0};
    Lexer lexer = {0};
    lexer_init(&lexer, data, length);
    while (!lexer_eof(&lexer)) {
        Token token = lexer_next(&lexer);
        if (token.type == TOKEN_ERROR) {
            lexer_print_error(&lexer, &token);
            throwError("Invalid Token");
        }
        token_print(&token);
        printf("\n");
        PARSER_OPCODE_TYPE opCode = parser_parse_token(parser, &token);
        if (opCode) {
            parser_opcodes_add(&codes, opCode);
        }
    }
    return codes;
}
bool parser_validate_opcodes(Parser *parser, ParsedOpCodes *opCodes) {
    (void)parser;
    (void)opCodes;
    return true;
}
void parser_opcodes_add(ParsedOpCodes *opCodes, PARSER_OPCODE_TYPE opCode) {
    if (opCodes->opCodesCount >= opCodes->opCodesMaxSize) {
        if (!opCodes->opCodesMaxSize) {
            opCodes->opCodesMaxSize = DEFAULT_OPCODE_COUNT;
        } else {
            opCodes->opCodesMaxSize *= 2;
        }
        opCodes->opCodes =
            realloc(opCodes->opCodes,
                    sizeof(PARSER_OPCODE_TYPE) * opCodes->opCodesMaxSize);
    }
    opCodes->opCodes[opCodes->opCodesCount++] = opCode;
}
void dataprimitivestable_add(DataPrimitivesTable *table,
                             DataPrimitive primitive) {
    if (table->count >= PARSER_MAX_DATAPRIMITIVES)
        throwError("Cannot define more than %d symbols",
                   PARSER_MAX_DATAPRIMITIVES);
    if (table->count >= table->maxCount) {
        if (!table->maxCount) {
            table->maxCount = PARSER_DEFAULT_DATAPRIMITIVES_COUNT;
        } else {
            table->maxCount *= 2;
            if (table->maxCount >= PARSER_MAX_DATAPRIMITIVES)
                table->maxCount = PARSER_MAX_DATAPRIMITIVES;
        }
        table->dataPrimitives = realloc(
            table->dataPrimitives, sizeof(DataPrimitive) * table->maxCount);
    }
    table->dataPrimitives[table->count++] = primitive;
}
void dataprimitivestable_free(DataPrimitivesTable *table) {
    for (size_t i = 0; i < table->count; i++) {
        switch (table->dataPrimitives[i].type) {
        case PRIM_RAW: {
            if (table->dataPrimitives[i].data.raw &&
                table->dataPrimitives[i].data.raw->data &&
                table->dataPrimitives[i].data.raw->size) {
                free(table->dataPrimitives[i].data.raw->data);
                free(table->dataPrimitives[i].data.raw);
            }
        } break;
        case PRIM_STRING: {
            if (table->dataPrimitives[i].data.string) {
                free(table->dataPrimitives[i].data.string);
            }
        } break;
        default:
            continue;
        }
    }
}
