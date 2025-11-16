#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "../include/pacman.h"

#define PACMAN_LOCK_TIMEOUT 600
#define PACMAN_LOCKFILE "/var/lib/pacman/db.lck"

extern char* run_cmd(const char *cmd);

static int pacman_lock_exists(void) {
    struct stat st;
    return (stat(PACMAN_LOCKFILE, &st) == 0);
}

static int pacman_wait_for_lock(void) {
    time_t start = time(NULL);
    while (pacman_lock_exists()) {
        usleep(250000);
        if (time(NULL) - start > PACMAN_LOCK_TIMEOUT) {
            return 0;
        }
    }
    return 1;
}

char* pacman_run(const char *args) {
    if (!args) {
        return strdup("Invalid arguments provided.");
    }
    if (pacman_lock_exists()) {
        fprintf(stderr, "pacman is already running, waiting...\n");
        if (!pacman_wait_for_lock()) {
            fprintf(stderr, "pacman lock did not clear after 10 minutes.\n");
            return strdup("pacman lock timeout.");
        }
    }
    size_t cmd_len = strlen("pacman ") + strlen(args) + 1;
    char *cmd = malloc(cmd_len);
    if (!cmd) {
        return strdup("Malloc failed.");
    }
    snprintf(cmd, cmd_len, "pacman %s", args);
    char *output = run_cmd(cmd);
    free(cmd);
    return output;
}

const PacmanStruct Pacman = {
    .run = pacman_run
};