
#ifndef DIRECTORY_OPS_H
#define DIRECTORY_OPS_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

/**
 * Directory operations module for file manager
 * Provides functionalities for directory manipulation and listing
 */

 /**
  * Lists all files and directories in the specified path
  * @param path The directory path to list contents from
  */
void list_directory(const char* path);

/**
 * Creates a new directory at the specified path
 * @param dir_name The name/path of the directory to create
 */
void create_directory(const char* dir_name);

/**
 * Deletes an empty directory at the specified path
 * @param dir_name The name/path of the directory to delete
 */
void delete_directory(const char* dir_name);

#endif
