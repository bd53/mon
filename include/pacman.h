#ifndef PACMAN_H
#define PACMAN_H

typedef char* (*handle)(const char *args);

typedef struct {
    handle run;
} PacmanStruct;

char* pacman_run(const char *args);

extern const PacmanStruct Pacman;

#endif