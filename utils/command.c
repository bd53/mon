#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 4096
#define LINE_BUFFER_SIZE 1024

char* run_cmd(const char *cmd) {
    FILE *fp = popen(cmd, "r");
    if (!fp) return strdup("Error reading data.");
    size_t capacity = INITIAL_BUFFER_SIZE;
    size_t size = 0;
    char *buffer = malloc(capacity);
    if (!buffer) {
        pclose(fp);
        return strdup("Memory allocation failed.");
    }
    buffer[0] = '\0';
    char line[LINE_BUFFER_SIZE];
    while (fgets(line, sizeof(line), fp)) {
        size_t line_len = strlen(line);
        while (size + line_len >= capacity) {
            capacity *= 2;
            char *new_buffer = realloc(buffer, capacity);
            if (!new_buffer) {
                free(buffer);
                pclose(fp);
                return strdup("Memory allocation failed.");
            }
            buffer = new_buffer;
        }
        strcpy(buffer + size, line);
        size += line_len;
    }
    pclose(fp);
    return buffer;
}