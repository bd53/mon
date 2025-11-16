#include <stdlib.h>
#include <string.h>
#include "info.h"

extern char* run_cmd(const char *cmd);

char* get_hostname(void) {
    return run_cmd("hostnamectl");
}

char* get_root_password(void) {
    return strdup("Root password is set (cannot display for security).");
}

char* get_user_account(void) {
    return run_cmd("cat /etc/passwd | grep -E '/home|/usr' | grep -v nologin");
}

char* get_profile(void) {
    return run_cmd("ls -la ~/.config 2>/dev/null | head -10");
}