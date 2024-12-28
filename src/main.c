#include "file_operations.h"
#include "directory_ops.h"
#include "permissions.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_menu() {
    printf("\nTerminal Based File Manager\n");
    printf("Commands:\n");
    printf("1. slist [path] - List directory contents\n");
    printf("2. screate [filename] - Create a new file\n");
    printf("3. smkdir [dirname] - Create a new directory\n");
    printf("4. sremove [path] - Remove file\n");
    printf("5. srmdir [path] - Remove directory\n");
    printf("6. scopy [src] [dest] - Copy file\n");
    printf("7. smove [src] [dest] - Move file\n");
    printf("8. sview [file] - View file contents\n");
    printf("9. ssearch [dir] [term] - Search for files\n");
    printf("10. sstat [path] - Show file/directory info\n");
    printf("11. schmod [path] [permissions] - Change permissions\n");
    printf("12. exit - Exit program\n");
    printf("> ");
}

int main() {
    char command[256], path1[128], path2[128];
    
    while (1) {
        print_menu();
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;  // Remove newline

        if (strncmp(command, "slist", 5) == 0) {
            sscanf(command, "slist %s", path1);
            list_directory(path1);
        }
        else if (strncmp(command, "screate", 7) == 0) {
            sscanf(command, "screate %s", path1);
            create_file(path1);
        }
        else if (strncmp(command, "smkdir", 6) == 0) {
            sscanf(command, "smkdir %s", path1);
            create_directory(path1);
        }
        else if (strncmp(command, "sremove", 7) == 0) {
            sscanf(command, "sremove %s", path1);
            delete_file(path1);
        }
        else if (strncmp(command, "srmdir", 6) == 0) {
            sscanf(command, "srmdir %s", path1);
            delete_directory(path1);
        }
        else if (strncmp(command, "scopy", 5) == 0) {
            sscanf(command, "scopy %s %s", path1, path2);
            copy_file(path1, path2);
        }
        else if (strncmp(command, "smove", 5) == 0) {
            sscanf(command, "smove %s %s", path1, path2);
            move_file(path1, path2);
        }
        else if (strncmp(command, "sview", 5) == 0) {
            sscanf(command, "sview %s", path1);
            view_file(path1);
        }
        else if (strncmp(command, "ssearch", 7) == 0) {
            sscanf(command, "ssearch %s %s", path1, path2);
            search_file(path1, path2);
        }
        else if (strncmp(command, "sstat", 5) == 0) {
            sscanf(command, "sstat %s", path1);
            show_file_info(path1);
        }
        else if (strncmp(command, "schmod", 6) == 0) {
            unsigned int permissions;
            sscanf(command, "schmod %s %o", path1, &permissions);
            change_permissions(path1, (mode_t)permissions);
        }
        else if (strcmp(command, "exit") == 0) {
            printf("Exiting program...\n");
            break;
        }
        else {
            printf("Invalid command. Please try again.\n");
        }
    }

    return 0;
}