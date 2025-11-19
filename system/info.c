#include <stdlib.h>
#include <string.h>
#include "info.h"

extern char* run_cmd(const char *cmd);

char* get_hostname(void) {
    return run_cmd("hostnamectl | awk -F': +' '{print $2}'");
}

char* get_root_password(void) {
    return strdup("Root password is set (cannot display for security).");
}

char* get_user_account(void) {
    return run_cmd("getent passwd | awk -F: '$6 ~ /^\\/home/ {print $1, $3, $6}'");
}