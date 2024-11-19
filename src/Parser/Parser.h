#include "../Common/Errors.h"
#include "SymbolTree.h"
#include <stdbool.h>
enum DataPrimitiveType {
    PRIM_NONE = 0,
    PRIM_INT8,
    PRIM_INT16,
    PRIM_INT32,
    PRIM_INT64,
    PRIM_FLOAT32,
    PRIM_FLOAT64,
    PRIM_STRING,
    PRIM_BOOL,
    PRIM_RAW,
};

typedef struct {
    size_t size;
    void *data;
} RawData;

typedef struct {
    enum DataPrimitiveType type;
    union {
        float float32;
        double float64;
        char int8;
        short int16;
        long int int32;
        long long int int64;
        char *string;
        bool boolean;
        RawData *raw;
    } data;
} DataPrimitive;

typedef struct {
    size_t count;
    size_t maxCount;
    DataPrimitive *dataPrimitives;
} DataPrimitivesTable;

typedef struct {
    DataPrimitivesTable dataPrimitivesTable;
    size_t opCodesCount;
    size_t opCodesMaxSize;
    PARSER_OPCODE_TYPE *opCodes;
} ParsedOpCodes;
typedef struct {
    size_t opCodeSize;
    size_t opCodesMaxSize;
    unsigned char *opCodes;
    ParserSymbolTree *symbolTree;
    DataPrimitivesTable dataPrimitivesTable;
} Parser;

void parser_init(Parser *parser);
void parser_destroy(Parser *parser);
void parser_register_symbols(Parser *parser, ParserSymbol *symbols,
                             size_t count);

ParsedOpCodes parser_parse(Parser *parser, const char *data, size_t length);
bool parser_validate_opcodes(Parser *parser, ParsedOpCodes *opCodes);
void parser_opcodes_add(ParsedOpCodes *opCodes, PARSER_OPCODE_TYPE opCode);
void dataprimitivestable_add(DataPrimitivesTable *table,
                             DataPrimitive primitive);
void dataprimitivestable_free(DataPrimitivesTable *table);
const char *serialize_parsed_op_codes(ParsedOpCodes *opCodes);
ParsedOpCodes deserialize_parsed_op_codes(const char *data);
