#include <string.h>
#include <stdlib.h>

char* safe_strdup(const char *str) {
    if (!str) return strdup("N/A");
    return strdup(str);
}

void trim_whitespace(char *str) {
    if (!str) return;
    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\n' || *end == '\t')) {
        end--;
    }
    *(end + 1) = '\0';
}