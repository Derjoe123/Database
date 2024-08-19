#include <stdbool.h>
#include <stdlib.h>
#define BTREE_ORDER 3
#define BTREE_MIN_KEYS (BTREE_ORDER / 2)

typedef struct BTreeNode {
    int keys[BTREE_ORDER - 1];
    struct BTreeNode *children[BTREE_ORDER];
    size_t childrenCount;
    bool leaf;
} BTreeNode;
BTreeNode *BTreeNode_new(int data, BTreeNode **children, int childrenCount);
void BTreeNode_free(BTreeNode *node);
void BTreeNode_print(BTreeNode *node);
void BTreeNode_splitChild(BTreeNode *parent, size_t index);

typedef struct {
    BTreeNode *root;
    size_t dataSize;
} BTree;

size_t BTree_insert(BTree *tree, void *data, size_t dataSize);
bool BTree_delete(BTree *tree, size_t index);
BTreeNode *BTree_search(BTreeNode *node, size_t key);
void BTree_free(BTree *tree);
void BTree_print(BTree *tree);
