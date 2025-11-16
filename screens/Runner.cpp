#include "Runner.hpp"
#include "Package.hpp"
#include "../ui/Terminal.hpp"
#include <iostream>
#include <unistd.h>

void ScreenRunner::run_detail_screen(const MenuItem& item, SystemMonitor& monitor, std::atomic<bool>& running) {
    bool is_packages = item.render == PackageScreen::render;
    while (running) {
        monitor.update();
        Terminal::clear();
        std::cout << Terminal::blue(" " + std::string(item.title) + " ") << "\n\n";
        bool should_exit = item.render(monitor);
        if (should_exit) return;
        std::cout << "\n" << (is_packages ? "Use ↑/↓ to scroll, ESC or q to return" : "Press ESC or q to return.") << "\n";
        if (is_packages) continue;
        for (int i = 0; i < 20; i++) {
            usleep(100000);
            if (Terminal::check_exit_key()) return;
        }
    }
}
