#ifndef TYPES_H
#define TYPES_H
#define MIN_DEGREE 2
#define FILE_TYPE 1
#define DIRECTORY_TYPE 2

// Estrutura adiantada para poder referenciar
struct Directory;

// Estrutura de arquivo
typedef struct File {
    char* name;
    char* content;
    int size;
} File;

// Estrutura de nó da árvore (arquivo ou diretório)
typedef struct TreeNode {
    char* name;
    int type;
    union {
        File* file;
        struct Directory* directory;
    } data;
} TreeNode;

// Nó da B-Tree
typedef struct BTreeNode {
    TreeNode** keys;
    int t;
    int num_keys;
    struct BTreeNode** children;
    int is_leaf;
} BTreeNode;

// Estrutura da B-Tree
typedef struct BTree {
    BTreeNode* root;
    int t;
} BTree;

// Estrutura de diretório
typedef struct Directory {
    BTree* tree;
    struct Directory* parent;
} Directory;

#endif
