#ifndef UI_H
#define UI_H

#include <ncurses.h>

void init_screen(void);
void cleanup_screen(void);
void show_detail(WINDOW *win, const char *title, const char *info);
void show_help(void);

#endif