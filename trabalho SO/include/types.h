#ifndef TYPES_H
#define TYPES_H
#define MIN_DEGREE 2
#define FILE_TYPE 1
#define DIRECTORY_TYPE 2

struct Directory;

typedef struct File {
    char* name;
    char* content;
    int size;
} File;

typedef struct TreeNode {
    char* name;
    int type;
    union {
        File* file;
        struct Directory* directory;
    } data;
} TreeNode;

typedef struct BTreeNode {
    TreeNode** keys;
    int t;
    int num_keys;
    struct BTreeNode** children;
    int is_leaf;
} BTreeNode;

typedef struct BTree {
    BTreeNode* root;
    int t;
} BTree;

typedef struct Directory {
    BTree* tree;
    struct Directory* parent;
} Directory;

#endif
