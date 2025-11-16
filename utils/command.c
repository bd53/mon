#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

char* run_cmd(const char *cmd) {
    static char buffer[MAX_OUTPUT];
    FILE *fp = popen(cmd, "r");
    if (!fp) return strdup("Error reading data");
    buffer[0] = '\0';
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        strncat(buffer, line, MAX_OUTPUT - strlen(buffer) - 1);
    }
    pclose(fp);
    return strdup(buffer);
}