#ifndef BTREE_H
#define BTREE_H

#include <stdbool.h>
#include "file.h"
#include "directory.h"

// Grau mínimo da árvore B
#define MIN_DEGREE 2

typedef struct BTreeNode {
    TreeNode** keys;
    int t; // Grau mínimo
    struct BTreeNode** children;
    int num_keys;
    bool is_leaf;
} BTreeNode;

typedef struct BTree {
    BTreeNode* root;
    int t; // Grau mínimo
} BTree;

// Criação
BTree* btree_create(int t);

// Operações
void btree_insert(BTree* tree, TreeNode* key);
TreeNode* btree_search(BTree* tree, const char* name);
void btree_delete(BTree* tree, const char* name);
void btree_traverse(BTree* tree);

#endif
