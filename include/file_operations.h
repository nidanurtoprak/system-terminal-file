#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

// File operations
void copy_file(const char* src, const char* dest);
void move_file(const char* src, const char* dest);
void view_file(const char* file_path);
void create_file(const char* filename);
void delete_file(const char* filename);
void search_file(const char* dir_path, const char* search_term);
void show_file_info(const char* path);

#endif