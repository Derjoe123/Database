#include "../Common/Errors.h"
#include "../Common/Settings.h"
#include <stdbool.h>
typedef struct {
    const char *name;
    PARSER_OPCODE_TYPE index;
} ParserSymbol;

typedef struct SymbolTreeNode {
    ParserSymbol *symbol;
    /* Trading space for time efficiency and simplicity */
    struct SymbolTreeNode *children[PARSER_SYMBOL_CHAR_COUNT];

} SymbolTreeNode;

typedef struct {
    SymbolTreeNode *root;
} ParserSymbolTree;

ParserSymbolTree *parser_symbol_tree_create(ParserSymbol *symbols,
                                            size_t count);
void parser_symbol_tree_destroy(ParserSymbolTree *tree);
void parser_symbol_tree_insert(ParserSymbolTree *tree, ParserSymbol *symbol);
void parser_symbol_tree_remove(ParserSymbolTree *tree, const char *name,
                               size_t nameLen);

ParserSymbol *parser_symbol_tree_get_symbol(ParserSymbolTree *tree,
                                            const char *name, size_t nameLen);

SymbolTreeNode *parser_symbol_tree_get_symbol_node(ParserSymbolTree *tree,
                                                   const char *name,
                                                   size_t nameLen);
