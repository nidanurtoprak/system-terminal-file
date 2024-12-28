#include "file_operations.h"  
#include "logger.h"  

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

void copy_file(const char* src, const char* dest) {  
    #ifdef _WIN32  
        HANDLE hSrc = CreateFile(src, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  
        if (hSrc == INVALID_HANDLE_VALUE) {  
            perror("Error opening source file");  
            log_action("Copy File", src, "Failed");  
            return;  
        }  

        HANDLE hDest = CreateFile(dest, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  
        if (hDest == INVALID_HANDLE_VALUE) {  
            perror("Error opening destination file");  
            CloseHandle(hSrc);  
            log_action("Copy File", dest, "Failed");  
            return;  
        }  

        char buffer[1024];  
        DWORD bytesRead, bytesWritten;  
        while (ReadFile(hSrc, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {  
            WriteFile(hDest, buffer, bytesRead, &bytesWritten, NULL);  
        }  

        CloseHandle(hSrc);  
        CloseHandle(hDest);  
    #else  
        int src_fd = open(src, O_RDONLY);  
        if (src_fd < 0) {  
            perror("Error opening source file");  
            log_action("Copy File", src, "Failed");  
            return;  
        }  

        int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);  
        if (dest_fd < 0) {  
            perror("Error opening destination file");  
            close(src_fd);  
            log_action("Copy File", dest, "Failed");  
            return;  
        }  

        char buffer[1024];  
        ssize_t bytes;  
        while ((bytes = read(src_fd, buffer, sizeof(buffer))) > 0) {  
            write(dest_fd, buffer, bytes);  
        }  

        close(src_fd);  
        close(dest_fd);  
    #endif  

    log_action("Copy File", src, "Success");  
}  

void move_file(const char* src, const char* dest) {  
    #ifdef _WIN32  
        if (MoveFile(src, dest)) {  
            printf("File moved successfully.\n");  
            log_action("Move File", src, "Success");  
        } else {  
            perror("Error moving file");  
            log_action("Move File", src, "Failed");  
        }  
    #else  
        if (rename(src, dest) == 0) {  
            printf("File moved successfully.\n");  
            log_action("Move File", src, "Success");  
        } else {  
            perror("Error moving file");  
            log_action("Move File", src, "Failed");  
        }  
    #endif  
}  

void view_file(const char* file_path) {  
    #ifdef _WIN32  
        HANDLE hFile = CreateFile(file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  
        if (hFile == INVALID_HANDLE_VALUE) {  
            perror("Error opening file");  
            log_action("View File", file_path, "Failed");  
            return;  
        }  

        char buffer[1024];  
        DWORD bytesRead;  
        // Okunan bayt sayısını kontrol et  
        while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {  
            // Elde edilen bayt sayısını yaz  
            DWORD bytesWritten;  
            WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, bytesRead, &bytesWritten, NULL);  
        }  

        CloseHandle(hFile);  
    #else  
        int fd = open(file_path, O_RDONLY);  
        if (fd < 0) {  
            perror("Error opening file");  
            log_action("View File", file_path, "Failed");  
            return;  
        }  

        char buffer[1024];  
        ssize_t bytes;  
        while ((bytes = read(fd, buffer, sizeof(buffer))) > 0) {  
            write(STDOUT_FILENO, buffer, bytes);  
        }  

        close(fd);  
    #endif  

    log_action("View File", file_path, "Success");  
}

void create_file(const char* filename) {  
    #ifdef _WIN32  
        HANDLE hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  
        if (hFile == INVALID_HANDLE_VALUE) {  
            perror("Error creating file");  
            log_action("Create File", filename, "Failed");  
            return;  
        }  
        CloseHandle(hFile);  
    #else  
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

void delete_file(const char* filename) {  
    #ifdef _WIN32  
        if (DeleteFile(filename)) {  
            printf("File deleted successfully\n");  
            log_action("Delete File", filename, "Success");  
        } else {  
            perror("Error deleting file");  
            log_action("Delete File", filename, "Failed");  
        }  
    #else  
        if (unlink(filename) == 0) {  
            printf("File deleted successfully\n");  
            log_action("Delete File", filename, "Success");  
        } else {  
            perror("Error deleting file");  
            log_action("Delete File", filename, "Failed");  
        }  
    #endif  
}  

void search_file(const char* dir_path, const char* search_term) {  
    #ifdef _WIN32  
        WIN32_FIND_DATA findFileData;  
        HANDLE hFind;  

        char search_pattern[MAX_PATH];  
        snprintf(search_pattern, sizeof(search_pattern), "%s\\*.*", dir_path);  

        hFind = FindFirstFile(search_pattern, &findFileData);  
        if (hFind == INVALID_HANDLE_VALUE) {  
            perror("Error opening directory");  
            return;  
        }  

        do {  
            if (strstr(findFileData.cFileName, search_term) != NULL) {  
                printf("Found: %s\n", findFileData.cFileName);  
            }  
        } while (FindNextFile(hFind, &findFileData) != 0);  
        
        FindClose(hFind);  
    #else  
        DIR* dir = opendir(dir_path);  
        if (!dir) {  
            perror("Error opening directory");  
            return;  
        }  

        struct dirent* entry;  
        while ((entry = readdir(dir)) != NULL) {  
            if (strstr(entry->d_name, search_term) != NULL) {  
                printf("Found: %s\n", entry->d_name);  
            }  
        }  
        closedir(dir);  
    #endif  
}  

void show_file_info(const char* path) {  
    struct stat file_stat;  
    if (stat(path, &file_stat) == -1) {  
        perror("Error getting file info");  
        return;  
    }  

    printf("File: %s\n", path);  
    printf("Size: %ld bytes\n", file_stat.st_size);  
    printf("Permissions: %o\n", file_stat.st_mode & 0777);  
    printf("Last access: %s", ctime(&file_stat.st_atime));  
}