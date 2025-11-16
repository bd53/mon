#include "Network.hpp"
#include "../ui/Terminal.hpp"
#include "../ui/Display.hpp"
#include <iostream>
#include <cstdio>
#include <memory>
#include <array>

bool NetworkScreen::render(SystemMonitor&) {
    std::array<char, 128> buffer;
    std::string result;
    std::cout << "\033[1mNetwork Interfaces:\033[0m\n";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("ip -br addr show | grep -v '^lo'", "r"), pclose);
    if (pipe) {
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            std::cout << buffer.data();
        }
    }
    std::cout << "\n\033[1mRouting Table:\033[0m\n";
    pipe.reset(popen("ip route show", "r"));
    if (pipe) {
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            std::cout << buffer.data();
        }
    }
    std::cout << "\n\033[1m/etc/resolv.conf:\033[0m\n";
    pipe.reset(popen("grep '^nameserver' /etc/resolv.conf 2>/dev/null || echo 'No nameservers configured'", "r"));
    if (pipe) {
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            std::cout << "" << buffer.data();
        }
    }
    std::cout << "\n\033[1mActive Connections:\033[0m\n";
    pipe.reset(popen("ss -tuln | head -15", "r"));
    if (pipe) {
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            std::cout << buffer.data();
        }
    }
    return Terminal::check_exit_key();
}
