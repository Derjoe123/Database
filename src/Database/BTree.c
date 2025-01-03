#include "BTree.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void BTreeNode_free(BTreeNode *node) {}
void BTreeNode_print(BTreeNode *node) {}
void BTreeNode_splitChild(BTreeNode *parent, size_t index) {}

BTreeNode *BTreeNode_new(int data, BTreeNode **children, int childrenCount) {
    assert(childrenCount < BTREE_ORDER);
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    node->childrenCount = childrenCount;
    for (int i = 0; i < childrenCount; ++i) {
        node->children[i] = children[i];
    }
    return node;
}

size_t BTree_insert(BTree *t, void *data, size_t dataSize) { return 0; }
bool BTree_delete(BTree *t, size_t key) { return false; }
BTreeNode *BTree_search(BTreeNode *startNode, size_t key) {
    assert(startNode);

    int i = 0;

    BTreeNode *node = startNode;

    while (true) {

        // loop all the children until we find the key that is higher (next
        // target node is before that)
        while (i < node->childrenCount && key > node->keys[i]) {
            i++;
        }

        // if we have found the key return the current node
        if (i < node->childrenCount && key == node->keys[i]) {
            return node;
        }

        // if the node is a leaf there are no children to search
        if (node->leaf) {
            return NULL;
        }

        assert(i < node->childrenCount);

        assert(node->children[i]);

        // search the child node with the first key that is higher
        node = node->children[i];

        i = 0;
    }
}
void BTree_free(BTree *t) {}
void BTree_print(BTree *t) {}
