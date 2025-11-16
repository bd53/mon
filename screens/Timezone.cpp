#include "Timezone.hpp"
#include <iostream>
#include <ctime>
#include <cstring>
#include <unistd.h>

bool TimezoneScreen::render(SystemMonitor&) {
    char link_target[256];
    std::string tz_name = "Unknown";
    ssize_t len = readlink("/etc/localtime", link_target, sizeof(link_target) - 1);
    if (len != -1) {
        link_target[len] = '\0';
        std::string path(link_target);
        size_t pos = path.find("/zoneinfo/");
        if (pos != std::string::npos) {
            tz_name = path.substr(pos + 10);
        }
    }
    time_t now = time(nullptr);
    struct tm* local_time = localtime(&now);
    char offset_buf[10], time_buf[100];
    strftime(offset_buf, sizeof(offset_buf), "%z", local_time);
    strftime(time_buf, sizeof(time_buf), "%A, %B %d, %Y %H:%M:%S", local_time);
    std::string tz_offset = offset_buf;
    if (tz_offset.length() == 5) {
        tz_offset = "UTC" + tz_offset.substr(0, 3) + ":" + tz_offset.substr(3, 2);
    }
    std::cout << "Current Timezone: " << tz_name << " (" << tz_offset << ")\n\n" << "Local Time: " << time_buf << "\n";
    return false;
}
