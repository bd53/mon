#include "info.h"

extern char* run_cmd(const char *cmd);

char* get_disk_config(void) {
    return run_cmd("lsblk -o NAME,TYPE,SIZE,FSTYPE,MOUNTPOINT,LABEL,UUID,PARTLABEL,PARTUUID");
}

char* get_swap(void) {
    return run_cmd("swapon --show --bytes --output NAME,TYPE,SIZE,USED,PRIO");
}

char* get_audio(void) {
    return run_cmd("pactl info 2>/dev/null; aplay -l 2>/dev/null | head -10");
}

char* get_network(void) {
    return run_cmd("nmcli dev wifi list 2>/dev/null | head -10 && echo '\n================================================' && ip -o addr show");
}