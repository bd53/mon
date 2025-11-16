#ifndef MENU_H
#define MENU_H

typedef struct {
    char *name;
    char *(*get_info)(void);
} MenuItem;

void init_menu_items(MenuItem *items);
void run_menu(void);

#endif