#include <ncurses.h>
#include <stdlib.h>
#include "menu.h"
#include "ui.h"
#include "config.h"

void run_menu(void) {
    MenuItem items[MAX_ITEMS];
    init_items(items);
    int highlight = 0;
    int ch;
    while (1) {
        clear();
        mvprintw(1, 4, "Press Ctrl+h for help");
        for (int i = 0; i < MAX_ITEMS; i++) {
            if (i == highlight) attron(COLOR_PAIR(1) | A_REVERSE);
            mvprintw(3 + i, 2, "%s%s", (i == highlight) ? "> " : "  ", items[i].name);
            if (i == highlight) attroff(COLOR_PAIR(1) | A_REVERSE);
        }
        int abort_line = 3 + MAX_ITEMS + 1;
        if (highlight == MAX_ITEMS) attron(COLOR_PAIR(1) | A_REVERSE);
        mvprintw(abort_line, 2, "%sAbort", (highlight == MAX_ITEMS) ? "> " : "  ");
        if (highlight == MAX_ITEMS) attroff(COLOR_PAIR(1) | A_REVERSE);
        refresh();
        ch = getch();
        switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + MAX_ITEMS + 1) % (MAX_ITEMS + 1);
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % (MAX_ITEMS + 1);
                break;
            case 10:
                if (highlight == MAX_ITEMS) return;
                {
                    char *info = items[highlight].get_info();
                    WINDOW *detail_win = newwin(LINES, COLS, 0, 0);
                    show_detail(detail_win, items[highlight].name, info);
                    delwin(detail_win);
                    free(info);
                }
                break;
            case 'q':
            case 'Q':
                return;
            case 8:
                show_help();
                break;
        }
    }
}