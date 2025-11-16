#ifndef MENU_HPP
#define MENU_HPP

#include "../Monitor.hpp"

struct MenuItem {
    const char* title;
    bool (*render)(SystemMonitor&);
};

class Menu {
    public:
        static void draw(int selected);
        static const MenuItem* get_items();
        static int get_item_count();
};

#endif