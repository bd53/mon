#include <ncurses.h>
#include "ui.h"

void init_screen(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
}

void cleanup_screen(void) {
    endwin();
}