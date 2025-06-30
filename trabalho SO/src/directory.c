#include <stdlib.h>
#include <string.h>
#include "directory.h"
#include "btree.h"
#include "types.h"
#define MIN_DEGREE 2


// directory.c
TreeNode* create_directory(const char* name, Directory* parent) {
    Directory* dir = malloc(sizeof(Directory));
    dir->tree = btree_create(MIN_DEGREE);
    dir->parent = parent;

    TreeNode* node = malloc(sizeof(TreeNode));
    node->name = strdup(name);
    node->type = DIRECTORY_TYPE;
    node->data.directory = dir;

    return node;
}


Directory* get_root_directory() {
    Directory* root = malloc(sizeof(Directory));
    root->tree = btree_create(MIN_DEGREE);
    root->parent = NULL;
    return root;
}


void list_directory_contents(Directory* dir) {
    if (dir && dir->tree) {
        btree_traverse(dir->tree);
    }
}

