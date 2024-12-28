#ifndef DIRECTORY_OPS_H
#define DIRECTORY_OPS_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

// Directory operations
void list_directory(const char* path);
void create_directory(const char* dir_name);
void delete_directory(const char* dir_name);

#endif
