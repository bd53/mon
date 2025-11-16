#include "Hostname.hpp"
#include "../ui/Terminal.hpp"
#include "../ui/Display.hpp"
#include <iostream>
#include <cstdio>
#include <memory>
#include <array>
#include <fstream>
#include <sstream>

bool HostnameScreen::render(SystemMonitor&) {
    std::array<char, 128> buffer;
    std::string result;
    std::cout << "Hostname: ";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("cat /etc/hostname 2>/dev/null || hostname", "r"), pclose);
    if (pipe) {
        if (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            std::cout << buffer.data();
        }
    }
    std::cout << "FQDN: ";
    pipe.reset(popen("hostnamectl --static 2>/dev/null || cat /etc/hostname 2>/dev/null || hostname", "r"));
    if (pipe) {
        if (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            std::cout << buffer.data();
        }
    }
    std::cout << "Domain: ";
    pipe.reset(popen("dnsdomainname 2>/dev/null || echo '(none)'", "r"));
    if (pipe) {
        if (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            std::cout << buffer.data();
        }
    }
    std::cout << "\n\033[1m/etc/hostname:\033[0m\n";
    std::ifstream hostname_file("/etc/hostname");
    if (hostname_file.is_open()) {
        std::string line;
        while (std::getline(hostname_file, line)) {
            std::cout << "" << line << "\n";
        }
        hostname_file.close();
    } else {
        std::cout << "  (unable to read file)\n";
    }
    std::cout << "\n\033[1mEntires in /etc/hosts:\033[0m\n";
    pipe.reset(popen("grep -v '^#' /etc/hosts | grep -v '^$' | head -10", "r"));
    if (pipe) {
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            std::cout << "" << buffer.data();
        }
    }
    return Terminal::check_exit_key();
}
