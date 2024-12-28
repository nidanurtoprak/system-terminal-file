#include "permissions.h"
#include "logger.h"

void change_permissions(const char* path, mode_t mode) {
    if (chmod(path, mode) == 0) {
        printf("Permissions changed successfully.\n");
        log_action("Change Permissions", path, "Success");
    }
    else {
        perror("Error changing permissions");
        log_action("Change Permissions", path, "Failed");
    }
}
