#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "btree.h"
#include "types.h"

BTree* btree_create(int t) {
    BTree* tree = malloc(sizeof(BTree));
    tree->t = t;

    BTreeNode* root = malloc(sizeof(BTreeNode));
    root->t = t;
    root->is_leaf = true;
    root->num_keys = 0;
    root->keys = malloc((2 * t - 1) * sizeof(TreeNode*));
    root->children = malloc((2 * t) * sizeof(BTreeNode*));

    tree->root = root;

    return tree;
}

void split_child(BTreeNode* parent, int i, BTreeNode* full_child) {
    int t = full_child->t;
    BTreeNode* new_child = malloc(sizeof(BTreeNode));
    new_child->t = t;
    new_child->is_leaf = full_child->is_leaf;
    new_child->num_keys = t - 1;

    new_child->keys = malloc((2 * t - 1) * sizeof(TreeNode*));
    new_child->children = malloc((2 * t) * sizeof(BTreeNode*));

    for (int j = 0; j < t - 1; j++) {
        new_child->keys[j] = full_child->keys[j + t];
    }

    if (!full_child->is_leaf) {
        for (int j = 0; j < t; j++) {
            new_child->children[j] = full_child->children[j + t];
        }
    }

    full_child->num_keys = t - 1;

    for (int j = parent->num_keys; j >= i + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[i + 1] = new_child;

    for (int j = parent->num_keys - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }

    parent->keys[i] = full_child->keys[t - 1];
    parent->num_keys++;
}

void insert_non_full(BTreeNode* node, TreeNode* key) {
    int i = node->num_keys - 1;

    if (node->is_leaf) {
        while (i >= 0 && strcmp(key->name, node->keys[i]->name) < 0) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && strcmp(key->name, node->keys[i]->name) < 0) {
            i--;
        }
        i++;

        if (node->children[i]->num_keys == 2 * node->t - 1) {
            split_child(node, i, node->children[i]);

            if (strcmp(key->name, node->keys[i]->name) > 0) {
                i++;
            }
        }

        insert_non_full(node->children[i], key);
    }
}

// Função de busca por nome para verificação de duplicidade
static TreeNode* btree_search_recursive(BTreeNode* node, const char* name) {
    int i = 0;
    while (i < node->num_keys && strcmp(name, node->keys[i]->name) > 0) {
        i++;
    }

    if (i < node->num_keys && strcmp(name, node->keys[i]->name) == 0) {
        return node->keys[i];
    }

    if (node->is_leaf) {
        return NULL;
    }

    return btree_search_recursive(node->children[i], name);
}

TreeNode* btree_search(BTree* tree, const char* name) {
    if (!tree || !tree->root) return NULL;
    return btree_search_recursive(tree->root, name);
}

void btree_insert(BTree* tree, TreeNode* key) {
    // Verifica duplicidade antes de inserir
    if (btree_search(tree, key->name) != NULL) {
        printf("Erro: já existe um item com o nome \"%s\" no diretório.\n", key->name);
        return;
    }

    BTreeNode* root = tree->root;

    if (root->num_keys == 2 * tree->t - 1) {
        BTreeNode* new_root = malloc(sizeof(BTreeNode));
        new_root->t = tree->t;
        new_root->is_leaf = false;
        new_root->num_keys = 0;
        new_root->keys = malloc((2 * tree->t - 1) * sizeof(TreeNode*));
        new_root->children = malloc((2 * tree->t) * sizeof(BTreeNode*));
        new_root->children[0] = root;

        tree->root = new_root;

        split_child(new_root, 0, root);
        insert_non_full(new_root, key);
    } else {
        insert_non_full(root, key);
    }
}

static void btree_traverse_recursive(BTreeNode* node) {
    int i;
    for (i = 0; i < node->num_keys; i++) {
        if (!node->is_leaf) {
            btree_traverse_recursive(node->children[i]);
        }

        TreeNode* item = node->keys[i];
        if (item->type == FILE_TYPE) {
            printf("[ARQUIVO] %s\n", item->name);
        } else if (item->type == DIRECTORY_TYPE) {
            printf("[PASTA] %s/\n", item->name);
        }
    }

    if (!node->is_leaf) {
        btree_traverse_recursive(node->children[i]);
    }
}

void change_directory(Directory** current, const char* path) {
    if (strcmp(path, "..") == 0) {
        if ((*current)->parent != NULL) {
            *current = (*current)->parent;
        } else {
            printf("Você já está no diretório raiz.\n");
        }
        return;
    }

    TreeNode* node = btree_search((*current)->tree, path);
    if (node && node->type == DIRECTORY_TYPE) {
        *current = node->data.directory;
    } else {
        printf("Diretório '%s' não encontrado.\n", path);
    }
}


void btree_traverse(BTree* tree) {
    if (!tree || !tree->root) return;
    btree_traverse_recursive(tree->root);
}

bool btree_delete(BTree* tree, const char* name) {
    if (!tree || !tree->root) return false;

    BTreeNode* node = tree->root;
    int i;
    for (i = 0; i < node->num_keys; i++) {
        if (strcmp(node->keys[i]->name, name) == 0) {
            TreeNode* item = node->keys[i];

            // Verifica se é diretório com conteúdo
            if (item->type == DIRECTORY_TYPE) {
                Directory* dir = item->data.directory;
                if (dir->tree && dir->tree->root->num_keys > 0) {
                    printf("Erro: o diretório \"%s\" não está vazio.\n", item->name);
                    return false;
                }
                // Libera estrutura de diretório vazia
                free(dir);
            } else if (item->type == FILE_TYPE) {
                // Libera conteúdo do arquivo
                free(item->data.file->content);
                free(item->data.file);
            }

            // Libera nome e estrutura do TreeNode
            free(item->name);
            free(item);

            // Desloca os itens restantes
            for (int j = i; j < node->num_keys - 1; j++) {
                node->keys[j] = node->keys[j + 1];
            }
            node->num_keys--;
            return true;
        }
    }

    return false; // Não encontrado
}

