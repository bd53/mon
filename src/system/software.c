#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "info.h"
#include "../../include/pacman.h"

extern char* run_cmd(const char *cmd);

char* get_language(void) {
    return run_cmd("localectl status | grep 'System Locale' | cut -d= -f2");
}

char* get_locale(void) {
    return run_cmd("locale -a | head -20");
}

char* get_mirrors(void) {
    return run_cmd("head -20 /etc/pacman.d/mirrorlist | grep '^Server'");
}

char* get_bootloader(void) {
    return run_cmd("bootctl status 2>/dev/null || grub-install --version 2>/dev/null");
}

char* get_kernel(void) {
    return run_cmd("uname -r");
}

char* get_kernels(void) {
    return Pacman.run("-Q | grep linux | grep -v linux-api");
}

char* get_packages(void) {
    return Pacman.run("-Qen | awk 'BEGIN {ORS=\"\"} {packages[NR]=$0} END {print \"total \" NR \"\\n\\n\"; for(i=1; i<=NR; i++) print packages[i] \"\\n\"}'");
}

char* get_timezone(void) {
    return run_cmd("timedatectl | grep 'Time zone' | awk '{print $3, $4, $5}'");
}

char* get_ntp(void) {
    return run_cmd("timedatectl | grep 'NTP' | awk '{print $3, $4}'");
}

char* get_system_errors(void) {
    return run_cmd("journalctl -p err -b --no-pager | tail -30");
}