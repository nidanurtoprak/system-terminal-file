#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <stdio.h>
#include <sys/stat.h>

// Permission operations
void change_permissions(const char* path, mode_t mode);

#endif