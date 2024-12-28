#include "logger.h"

void log_action(const char* action, const char* path, const char* status) {
    FILE* log_file = fopen("file_manager.log", "a");
    if (!log_file) {
        perror("Error opening log file");
        return;
    }

    time_t now = time(NULL);
    fprintf(log_file, "[%s] %s: %s (%s)\n", ctime(&now), action, path, status);
    fclose(log_file);
}