#include "SymbolTree.h"
#include <ctype.h>
#include <stdlib.h>
ParserSymbolTree *parser_symbol_tree_create(ParserSymbol *symbols,
                                            size_t count) {
    ParserSymbolTree *tree = malloc(sizeof(ParserSymbolTree));
    for (size_t i = 0; i < count; i++) {
        if (!symbols[i].name) {
            throwError("tried adding empty symbol name to symbol tree");
        }
        parser_symbol_tree_insert(tree, &symbols[i]);
    }
    return tree;
}
void node_destroy(SymbolTreeNode *node) {

    for (int i = 0; i < PARSER_SYMBOL_CHAR_COUNT; i++) {
        if (node->children[i])
            node_destroy(node->children[i]);
    }
    if (node->symbol) {
        free(node->symbol);
        node->symbol = NULL;
    }
    free(node);
}
bool valid_symbol_char(char c) { return isalpha(c) || isdigit(c) || c == '_'; }
size_t convert_char_to_index(char c) {
    // convert ascii to 0 based
    // 0-25 a-z
    // 26-35 A-Z
    // 36-45 0-9
    // 46 _
    size_t index = 0;
    if (isalpha(c)) {
        index = (size_t)c - (isupper(c) ? 'A' : 'a');
    } else if (isdigit(c)) {
        index = 26 + (size_t)c - '0';
    } else if (c == '_') {
        index = 36;
    }
    return index;
}
void parser_symbol_tree_destroy(ParserSymbolTree *tree) {
    node_destroy(tree->root);
    free(tree);
}
void node_alloc_insert(SymbolTreeNode **parentNodes, size_t index) {
    parentNodes[index] = malloc(sizeof(SymbolTreeNode));
}
void parser_symbol_tree_insert(ParserSymbolTree *tree, ParserSymbol *symbol) {
    if (!symbol->index) {
        throwError("Tried adding symbol with index 0 to symbol tree");
        return;
    }
    size_t nameLen = strlen(symbol->name);
    if (nameLen < 1) {
        throwError("tried adding empty symbol to symboltree");
        return;
    }
    if (!tree->root) {
        tree->root = malloc(sizeof(SymbolTreeNode));
    }
    SymbolTreeNode *prevNode = tree->root;
    for (size_t i = 0; i < nameLen; i++) {
        if (!valid_symbol_char(symbol->name[i])) {
            throwError("tried adding invalid symbol to symboltree");
            return;
        }
        size_t cur = convert_char_to_index(symbol->name[i]);
        if (!prevNode->children[cur]) {
            node_alloc_insert(prevNode->children, cur);
        }
        prevNode = prevNode->children[cur];
    }
    if (prevNode->symbol) {
        throwError("tried adding existing symbol to symboltree");
        return;
    }
    ParserSymbol *newSymbol = malloc(sizeof(ParserSymbol));
    memcpy(newSymbol, symbol, sizeof(ParserSymbol));
    prevNode->symbol = newSymbol;
}
void parser_symbol_tree_remove(ParserSymbolTree *tree, const char *name,
                               size_t nameLen) {
    SymbolTreeNode *parent =
        parser_symbol_tree_get_symbol_node(tree, name, nameLen);
    free(parent->symbol);
    parent->symbol = NULL;
}

ParserSymbol *parser_symbol_tree_get_symbol(ParserSymbolTree *tree,
                                            const char *name, size_t nameLen) {
    SymbolTreeNode *parent =
        parser_symbol_tree_get_symbol_node(tree, name, nameLen);
    if (!parent) {
        return NULL;
    }
    return parent->symbol;
}

SymbolTreeNode *parser_symbol_tree_get_symbol_node(ParserSymbolTree *tree,
                                                   const char *name,
                                                   size_t nameLen) {
    if (!tree || !name)
        return NULL;
    SymbolTreeNode *curNode = NULL;

    if (nameLen < 1) {
        throwError("tried finding empty name in symboltree");
        return NULL;
    }

    if (!tree->root) {
        printf("Root is null\n");
        return NULL;
    }
    curNode = tree->root;
    for (size_t i = 0; i < nameLen; i++) {
        if (!valid_symbol_char(name[i])) {
            return NULL;
        }
        size_t cur = convert_char_to_index(name[i]);
        if (!curNode->children[cur]) {
            printf("Child is null [%c] in [%.*s]\n", name[i], (int)nameLen,
                   name);
            return NULL;
        }
        curNode = curNode->children[cur];
    }
    return curNode;
}
