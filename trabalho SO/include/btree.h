#ifndef BTREE_H
#define BTREE_H
#include <stdbool.h>
#include "types.h"

BTree* btree_create(int t);
void btree_insert(BTree* tree, TreeNode* key);
void btree_traverse(BTree* tree);
TreeNode* btree_search(BTree* tree, const char* name);
bool btree_delete(BTree* tree, const char* name);

#endif
