#include "directory_ops.h"
#include "logger.h"
#include <sys/stat.h>
#include <unistd.h>    // For mkdir on Unix-based systems
#include <windows.h>   // For Windows systems

/**
 * Lists contents of a directory
 * Uses opendir/readdir for cross-platform directory listing
 */
void list_directory(const char* path) {
    // Attempt to open the directory
    DIR* dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        log_action("List Directory", path, "Failed");
        return;
    }

    // Read and print directory entries
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Cleanup and logging
    closedir(dir);
    log_action("List Directory", path, "Success");
}

/**
 * Creates a new directory with platform-specific implementations
 * Handles both Windows and Unix-like systems
 */
void create_directory(const char* dir_name) {
#ifdef _WIN32
    // Windows-specific directory creation
    DWORD ftyp = GetFileAttributesA(dir_name);
    if (ftyp == INVALID_FILE_ATTRIBUTES || !(ftyp & FILE_ATTRIBUTE_DIRECTORY)) {
        if (CreateDirectory(dir_name, NULL)) {
            printf("Directory created successfully.\n");
            log_action("Create Directory", dir_name, "Success");
        }
        else {
            perror("Error creating directory");
            log_action("Create Directory", dir_name, "Failed");
        }
    }
    else {
        printf("Directory already exists.\n");
        log_action("Create Directory", dir_name, "Already Exists");
    }
#else
    // Unix-like systems directory creation
    if (mkdir(dir_name, 0755) == 0) {
        printf("Directory created successfully.\n");
        log_action("Create Directory", dir_name, "Success");
    }
    else if (errno == EEXIST) {
        printf("Directory already exists.\n");
        log_action("Create Directory", dir_name, "Already Exists");
    }
    else {
        perror("Error creating directory");
        log_action("Create Directory", dir_name, "Failed");
    }
#endif
}

/**
 * Deletes an empty directory
 * Uses rmdir system call for both platforms
 */
void delete_directory(const char* dir_name) {
    if (rmdir(dir_name) == 0) {
        printf("Directory deleted successfully.\n");
        log_action("Delete Directory", dir_name, "Success");
    }
    else {
        perror("Error deleting directory");
        log_action("Delete Directory", dir_name, "Failed");
    }
}
