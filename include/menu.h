#ifndef MENU_H
#define MENU_H

typedef struct {
    char *name;
    char *(*get_info)(void);
} MenuItem;

extern MenuItem MENU_ITEMS[];
extern const int MENU_ITEM_COUNT;

void run_menu(void);

#endif