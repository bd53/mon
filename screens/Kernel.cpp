#include "Kernel.hpp"
#include <iostream>
#include <iomanip>
#include <sys/utsname.h>
#include <cstdio>

bool KernelScreen::render(SystemMonitor&) {
    struct utsname sys_info;
    if (uname(&sys_info) == 0) {
        std::cout << "Name: " << sys_info.sysname << "\n";
        std::cout << "Release: " << sys_info.release << "\n";
        std::cout << "Version: " << sys_info.version << "\n";
        std::cout << "Machine: " << sys_info.machine << "\n";
        std::cout << "Hostname: " << sys_info.nodename << "\n";
    }
    std::cout << "\n\033[1mInstalled:\033[0m\n";
    FILE* pipe = popen("ls -1 /boot/vmlinuz-* 2>/dev/null | sed 's|/boot/vmlinuz-||'", "r");
    if (pipe) {
        char buffer[256];
        int count = 0;
        while (fgets(buffer, sizeof(buffer), pipe)) {
            std::string kernel(buffer);
            kernel.erase(kernel.find_last_not_of("\n\r") + 1);
            if (!kernel.empty()) {
                bool is_current = (kernel == sys_info.release);
                std::cout << "" << kernel;
                if (is_current) {
                    std::cout << "(current)";
                }
                std::cout << "\n";
                count++;
            }
        }
        pclose(pipe);
        if (count == 0) {
            std::cout << "No images found in /boot\n";
        }
    } else {
        std::cout << "Unable to list installed kernels\n";
    }
    return false;
}
