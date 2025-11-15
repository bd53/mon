#include "Package.hpp"
#include "../ui/Terminal.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdio>
#include <unistd.h>
#include <sys/select.h>

bool PackageScreen::render(SystemMonitor&) {
    static int scroll_offset = 0;
    static std::vector<std::pair<std::string, std::string>> packages;
    static bool loaded = false;
    if (!loaded) {
        Terminal::clear();
        std::cout << Terminal::blue(" Additional packages ") << "\n\n" << "Loading packages... (this may take a moment)\n";
        FILE* pipe = popen("pacman -Q 2>/dev/null", "r");
        if (pipe) {
            char buffer[512];
            while (fgets(buffer, sizeof(buffer), pipe)) {
                std::string line(buffer);
                line.erase(line.find_last_not_of("\n\r") + 1);
                size_t space_pos = line.find(' ');
                if (space_pos != std::string::npos) {
                    packages.push_back({line.substr(0, space_pos), line.substr(space_pos + 1)});
                }
            }
            pclose(pipe);
        }
        loaded = true;
        scroll_offset = 0;
    }
    std::cout << std::left << std::setw(40) << "PACKAGE" << "VERSION\n" << "--------------------------------------------------------------------------------\n";
    const int max_display = 15;
    int end = std::min(scroll_offset + max_display, static_cast<int>(packages.size()));
    for (int i = scroll_offset; i < end; i++) {
        std::string name = packages[i].first.length() > 39 ? packages[i].first.substr(0, 36) + "..." : packages[i].first;
        std::string version = packages[i].second.length() > 35 ? packages[i].second.substr(0, 32) + "..." : packages[i].second;
        std::cout << std::left << std::setw(40) << name << version << "\n";
    }
    if (end < static_cast<int>(packages.size())) {
        std::cout << "\n... " << (packages.size() - end) << " more below\n";
    }
    if (scroll_offset > 0) {
        std::cout << "... " << scroll_offset << " packages above\n";
    }
    std::cout << "\nTotal packages: " << packages.size() << "\n";
    fd_set fds;
    struct timeval tv = {0, 200000};
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0) {
        char c = getchar();
        if (c == 'q' || c == 'Q') return true;
        if (c == 27) {
            tv = {0, 10000};
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);
            if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0 && getchar() == '[') {
                char d = getchar();
                if (d == 'A') scroll_offset = std::max(0, scroll_offset - 1);
                else if (d == 'B') scroll_offset = std::min(std::max(0, static_cast<int>(packages.size()) - max_display), scroll_offset + 1);
            } else {
                return true;
            }
        }
    }
    return false;
}