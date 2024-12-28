// Header file includes for file operations and logging
#include "file_operations.h"  
#include "logger.h"  

// Conditional compilation for Windows vs Unix-like systems
#ifdef _WIN32  
#include <windows.h>  
#include <io.h>  
#include <stdio.h>  
#include <string.h>  
#else  
#include <fcntl.h>  
#include <unistd.h>  
#include <dirent.h>  
#include <sys/stat.h>  
#include <stdio.h>  
#include <string.h>  
#endif  

/**
 * Copies a file from source to destination
 * @param src Source file path
 * @param dest Destination file path
 */
void copy_file(const char* src, const char* dest) {
#ifdef _WIN32  
    // Windows implementation using WinAPI
    HANDLE hSrc = CreateFile(src, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSrc == INVALID_HANDLE_VALUE) {
        perror("Error opening source file");
        log_action("Copy File", src, "Failed");
        return;
    }

    // Create destination file with write access
    HANDLE hDest = CreateFile(dest, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDest == INVALID_HANDLE_VALUE) {
        perror("Error opening destination file");
        CloseHandle(hSrc);
        log_action("Copy File", dest, "Failed");
        return;
    }

    // Copy file contents in chunks using a buffer
    char buffer[1024];
    DWORD bytesRead, bytesWritten;
    while (ReadFile(hSrc, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
        WriteFile(hDest, buffer, bytesRead, &bytesWritten, NULL);
    }

    // Clean up handles
    CloseHandle(hSrc);
    CloseHandle(hDest);
#else  
    // Unix implementation using system calls
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) {
        perror("Error opening source file");
        log_action("Copy File", src, "Failed");
        return;
    }

    // Create destination file with read/write permissions
    int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("Error opening destination file");
        close(src_fd);
        log_action("Copy File", dest, "Failed");
        return;
    }

    // Copy file contents in chunks
    char buffer[1024];
    ssize_t bytes;
    while ((bytes = read(src_fd, buffer, sizeof(buffer))) > 0) {
        write(dest_fd, buffer, bytes);
    }

    // Close file descriptors
    close(src_fd);
    close(dest_fd);
#endif  

    log_action("Copy File", src, "Success");
}

/**
 * Moves/renames a file from source to destination
 * @param src Source file path
 * @param dest Destination file path
 */
void move_file(const char* src, const char* dest) {
#ifdef _WIN32  
    // Windows implementation using MoveFile function
    if (MoveFile(src, dest)) {
        printf("File moved successfully.\n");
        log_action("Move File", src, "Success");
    }
    else {
        perror("Error moving file");
        log_action("Move File", src, "Failed");
    }
#else  
    // Unix implementation using rename function
    if (rename(src, dest) == 0) {
        printf("File moved successfully.\n");
        log_action("Move File", src, "Success");
    }
    else {
        perror("Error moving file");
        log_action("Move File", src, "Failed");
    }
#endif  
}

/**
 * Displays the contents of a file
 * @param file_path Path to the file to be viewed
 */
void view_file(const char* file_path) {
#ifdef _WIN32  
    // Open file for reading in Windows
    HANDLE hFile = CreateFile(file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        perror("Error opening file");
        log_action("View File", file_path, "Failed");
        return;
    }

    // Read and display file contents
    char buffer[1024];
    DWORD bytesRead;
    while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
        DWORD bytesWritten;
        WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, bytesRead, &bytesWritten, NULL);
    }

    CloseHandle(hFile);
#else  
    // Open file for reading in Unix
    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        log_action("View File", file_path, "Failed");
        return;
    }

    // Read and display file contents
    char buffer[1024];
    ssize_t bytes;
    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes);
    }

    close(fd);
#endif  

    log_action("View File", file_path, "Success");
}

/**
 * Creates a new empty file
 * @param filename Name of the file to create
 */
void create_file(const char* filename) {
#ifdef _WIN32  
    // Create new file in Windows
    HANDLE hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        perror("Error creating file");
        log_action("Create File", filename, "Failed");
        return;
    }
    CloseHandle(hFile);
#else  
    // Create new file in Unix with permissions 0644
    int fd = creat(filename, 0644);
    if (fd < 0) {
        perror("Error creating file");
        log_action("Create File", filename, "Failed");
        return;
    }
    close(fd);
#endif  

    log_action("Create File", filename, "Success");
}

/**
 * Deletes a file from the filesystem
 * @param filename Name of the file to delete
 */
void delete_file(const char* filename) {
#ifdef _WIN32  
    // Delete file in Windows
    if (DeleteFile(filename)) {
        printf("File deleted successfully\n");
        log_action("Delete File", filename, "Success");
    }
    else {
        perror("Error deleting file");
        log_action("Delete File", filename, "Failed");
    }
#else  
    // Delete file in Unix
    if (unlink(filename) == 0) {
        printf("File deleted successfully\n");
        log_action("Delete File", filename, "Success");
    }
    else {
        perror("Error deleting file");
        log_action("Delete File", filename, "Failed");
    }
#endif  
}

/**
 * Searches for files containing the search term in the specified directory
 * @param dir_path Directory to search in
 * @param search_term Term to search for in filenames
 */
void search_file(const char* dir_path, const char* search_term) {
#ifdef _WIN32  
    // Windows directory search implementation
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;

    char search_pattern[MAX_PATH];
    snprintf(search_pattern, sizeof(search_pattern), "%s\\.", dir_path);

    hFind = FindFirstFile(search_pattern, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        perror("Error opening directory");
        return;
    }

    // Iterate through directory entries
    do {
        if (strstr(findFileData.cFileName, search_term) != NULL) {
            printf("Found: %s\n", findFileData.cFileName);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
#else  
    // Unix directory search implementation
    DIR* dir = opendir(dir_path);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    // Iterate through directory entries
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, search_term) != NULL) {
            printf("Found: %s\n", entry->d_name);
        }
    }
    closedir(dir);
#endif  
}

/**
 * Displays detailed information about a file
 * @param path Path to the file
 */
void show_file_info(const char* path) {
    // Get file statistics
    struct stat file_stat;
    if (stat(path, &file_stat) == -1) {
        perror("Error getting file info");
        return;
    }

    // Display file information
    printf("File: %s\n", path);
    printf("Size: %ld bytes\n", file_stat.st_size);
    printf("Permissions: %o\n", file_stat.st_mode & 0777);
    printf("Last access: %s", ctime(&file_stat.st_atime));
}