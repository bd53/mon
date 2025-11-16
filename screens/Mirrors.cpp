#include "Mirrors.hpp"
#include "../ui/Terminal.hpp"
#include "../ui/Display.hpp"
#include "../utils/Pipe.hpp"
#include <iostream>
#include <cstdio>
#include <memory>
#include <array>

bool MirrorsScreen::render(SystemMonitor&) {
    std::cout << "\033[1mMirror Configuration:\033[0m\n";
    std::cout << "Location: /etc/pacman.d/mirrorlist\n";
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, PipeDeleter> pipe(popen("grep -v '^#' /etc/pacman.d/mirrorlist | grep Server | head -10", "r"));
    if (pipe) {
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
    }
    if (result.empty()) {
        std::cout << "No active mirrors found.\n";
    } else {
        std::cout << result;
    }
    std::cout << "\n" << "\033[1mConfigured Repositories:\033[0m\n";
    result.clear();
    pipe.reset(popen("grep -E '^\\[.*\\]' /etc/pacman.conf | grep -v '#'", "r"));
    if (pipe) {
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
    }
    if (result.empty()) {
        std::cout << "No repositories found.\n";
    } else {
        std::cout << result;
    }
    return Terminal::check_exit_key();
}