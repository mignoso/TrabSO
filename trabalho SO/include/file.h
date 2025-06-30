#ifndef FILE_H
#define FILE_H
#include "types.h"

TreeNode* create_txt_file(const char* name, const char* content);
void delete_txt_file(BTree* tree, const char* name);

#endif
