#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "ui.h"

void show_detail(WINDOW *win, const char *title, const char *info) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "%s", title);
    mvwhline(win, 2, 1, ACS_HLINE, COLS - 2);
    int y = 4;
    char *info_copy = strdup(info);
    char *line = strtok(info_copy, "\n");
    while (line && y < LINES - 3) {
        mvwprintw(win, y++, 2, "%.*s", COLS - 4, line);
        line = strtok(NULL, "\n");
    }
    free(info_copy);
    mvwprintw(win, LINES - 2, 2, "Press any key to return...");
    wrefresh(win);
    wgetch(win);
}

void show_help(void) {
    WINDOW *help_win = newwin(LINES, COLS, 0, 0);
    show_detail(help_win, "Help", "Arrow Keys: Navigate menu\n" "ENTER: View configuration details\n" "Q: Quit\n\n" "Data is gathered from system commands and configuration files.");
    delwin(help_win);
}