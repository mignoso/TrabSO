#ifndef DIRECTORY_H
#define DIRECTORY_H
#include "types.h"

TreeNode* create_directory(const char* name, Directory* parent);
void delete_directory(BTree* tree, const char* name);
Directory* get_root_directory();
void change_directory(Directory** current, const char* path);
void list_directory_contents(Directory* dir);

#endif
