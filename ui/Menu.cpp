#include "Menu.hpp"
#include "Terminal.hpp"
#include "../screens/System.hpp"
#include "../screens/CPU.hpp"
#include "../screens/Memory.hpp"
#include "../screens/Process.hpp"
#include "../screens/Profile.hpp"
#include "../screens/Kernel.hpp"
#include "../screens/Package.hpp"
#include "../screens/Timezone.hpp"
#include <iostream>
#include <cstring>

static const MenuItem MENU_ITEMS[] = {
    {"System overview", SystemScreen::render},
    {"CPU", CpuScreen::render},
    {"Memory", MemoryScreen::render},
    {"Top processes (by memory)", ProcessScreen::render},
    {"Profile", ProfileScreen::render},
    {"Kernels", KernelScreen::render},
    {"Additional packages", PackageScreen::render},
    {"Timezone", TimezoneScreen::render}
};

static const int MENU_COUNT = 8;

void Menu::draw(int selected) {
    Terminal::clear();
    std::cout << "Press ↑/↓ and ENTER for info\n\n";
    for (int i = 0; i < MENU_COUNT; i++) {
        std::string item = std::string(MENU_ITEMS[i].title) + std::string(33 - strlen(MENU_ITEMS[i].title), ' ') + "+";
        std::cout << (i == selected ? Terminal::blue(" " + item + " ") : " " + item) << "\n";
    }
}

const MenuItem* Menu::get_items() {
    return MENU_ITEMS;
}

int Menu::get_item_count() {
    return MENU_COUNT;
}