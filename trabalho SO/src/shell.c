#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/shell.h"
#include "../include/directory.h"
#include "../include/file.h"
#include "../include/btree.h"
#include <stdbool.h> 
void shell_loop() {
    Directory* root = get_root_directory();
    Directory* current = root;

    char command[100];
    while (1) {
        printf("SO> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strncmp(command, "mkdir ", 6) == 0) {
            TreeNode* dir = create_directory(command + 6, current);
            btree_insert(current->tree, dir);
        } else if (strncmp(command, "touch ", 6) == 0) {
            TreeNode* file = create_txt_file(command + 6, "");
            btree_insert(current->tree, file);
        } else if (strcmp(command, "ls") == 0) {
            list_directory_contents(current);
        } else if (strncmp(command, "cd ", 3) == 0) {
            char* path = command + 3;
            change_directory(&current, path);
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else if (strncmp(command, "rm ", 3) == 0) {
            char* name = command + 3;
            bool success = btree_delete(current->tree, name);
            if (success) {
                printf("Removido com sucesso: %s\n", name);
            } else {
                printf("Item \"%s\" não encontrado para remoção.\n", name);
            }
        }
    }
}
