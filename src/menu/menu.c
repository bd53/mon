#include <ncurses.h>
#include <stdlib.h>
#include "menu.h"
#include "ui.h"

void run_menu(void) {
    int highlight = 0;
    int ch;
    while (1) {
        clear();
        mvprintw(1, 4, "Press Ctrl+h for help");
        for (int i = 0; i < MENU_ITEM_COUNT; i++) {
            if (i == highlight) attron(COLOR_PAIR(1) | A_REVERSE);
            mvprintw(3 + i, 2, "%s%s", (i == highlight) ? "> " : "  ", MENU_ITEMS[i].name);
            if (i == highlight) attroff(COLOR_PAIR(1) | A_REVERSE);
        }
        int abort_line = 3 + MENU_ITEM_COUNT + 1;
        if (highlight == MENU_ITEM_COUNT) attron(COLOR_PAIR(1) | A_REVERSE);
        mvprintw(abort_line, 2, "%sAbort", (highlight == MENU_ITEM_COUNT) ? "> " : "  ");
        if (highlight == MENU_ITEM_COUNT) attroff(COLOR_PAIR(1) | A_REVERSE);
        refresh();
        ch = getch();
        switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + MENU_ITEM_COUNT + 1) % (MENU_ITEM_COUNT + 1);
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % (MENU_ITEM_COUNT + 1);
                break;
            case 10:
                if (highlight == MENU_ITEM_COUNT) return;
                {
                    char *info = MENU_ITEMS[highlight].get_info();
                    WINDOW *detail_win = newwin(LINES, COLS, 0, 0);
                    show_detail(detail_win, MENU_ITEMS[highlight].name, info);
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