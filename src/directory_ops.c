// Header files for directory operations and logging
#include "directory_ops.h"  
#include "logger.h"  

// System headers for file and directory operations
#include <sys/stat.h>  
#include <unistd.h>  // For mkdir on Unix-based systems  
#include <windows.h> // For Windows systems  

/**
 * Lists all entries in a specified directory
 * @param path Path to the directory to list
 */
void list_directory(const char* path) {
    // Attempt to open the directory
    DIR* dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        log_action("List Directory", path, "Failed");
        return;
    }

    // Iterate through directory entries and print each name
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Clean up and close directory handle
    closedir(dir);
    log_action("List Directory", path, "Success");
}

/**
 * Creates a new directory with specified name
 * @param dir_name Name/path of the directory to create
 * Handles both Windows and Unix-like systems with appropriate permissions
 */
void create_directory(const char* dir_name) {
#ifdef _WIN32  
    // Windows implementation
    // Check if directory already exists  
    DWORD ftyp = GetFileAttributesA(dir_name);
    if (ftyp == INVALID_FILE_ATTRIBUTES || !(ftyp & FILE_ATTRIBUTE_DIRECTORY)) {
        // Attempt to create directory if it doesn't exist
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
        // Directory already exists
        printf("Directory already exists.\n");
        log_action("Create Directory", dir_name, "Already Exists");
    }
#else  
    // Unix implementation
    // Attempt to create directory with read/write/execute permissions (0755)
    if (mkdir(dir_name, 0755) == 0) {
        printf("Directory created successfully.\n");
        log_action("Create Directory", dir_name, "Success");
    }
    else if (errno == EEXIST) {
        // Handle case where directory already exists
        printf("Directory already exists.\n");
        log_action("Create Directory", dir_name, "Already Exists");
    }
    else {
        // Handle other errors
        perror("Error creating directory");
        log_action("Create Directory", dir_name, "Failed");
    }
#endif  
}

/**
 * Deletes an empty directory
 * @param dir_name Name/path of the directory to delete
 * Note: Directory must be empty for deletion to succeed
 */
void delete_directory(const char* dir_name) {
    // Attempt to remove directory
    if (rmdir(dir_name) == 0) {
        printf("Directory deleted successfully.\n");
        log_action("Delete Directory", dir_name, "Success");
    }
    else {
        // Handle deletion errors (directory not empty, permissions, etc.)
        perror("Error deleting directory");
        log_action("Delete Directory", dir_name, "Failed");
    }
}