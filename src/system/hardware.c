#include "info.h"

extern char* run_cmd(const char *cmd);

char* get_disk_config(void) {
    return run_cmd("lsblk -o NAME,SIZE,TYPE,MOUNTPOINT,FSTYPE");
}

char* get_swap(void) {
    return run_cmd("swapon --show");
}

char* get_audio(void) {
    return run_cmd("pactl info 2>/dev/null; aplay -l 2>/dev/null | head -10");
}

char* get_network(void) {
    return run_cmd("ip addr show | grep -E 'inet |^[0-9]'");
}