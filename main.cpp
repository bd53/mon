#include "Monitor.hpp"
#include "ui/Terminal.hpp"
#include "ui/Menu.hpp"
#include "screens/Runner.hpp"
#include <iostream>
#include <atomic>
#include <csignal>

std::atomic<bool> running{true};

void signal_handler(int) {
    running = false;
}

int main() {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    Terminal::init();
    Terminal::enable_raw_mode();
    SystemMonitor monitor(60);
    monitor.update();
    int selected = 0;
    const int menu_count = Menu::get_item_count();
    const MenuItem* menu_items = Menu::get_items();
    while (running) {
        Menu::draw(selected);
        char c = getchar();
        if (c == 27 && getchar() == '[') {
            char d = getchar();
            if (d == 'A') selected = (selected + menu_count - 1) % menu_count;
            if (d == 'B') selected = (selected + 1) % menu_count;
        }
        if (c == '\n' || c == '\r') {
            ScreenRunner::run_detail_screen(menu_items[selected], monitor, running);
        }
    }
    Terminal::disable_raw_mode();
    Terminal::clear();
    std::cout << "Shutting down...\n";
    return 0;
}
