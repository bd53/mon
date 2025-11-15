#include "System.hpp"
#include <iostream>

extern "C" {
    #include "../sysinfo.h"
}

bool SystemScreen::render(SystemMonitor& monitor) {
    auto snapshot = monitor.get_current_snapshot();
    std::cout << "Uptime: " << monitor.format_uptime() << "\n" << "CPUs: " << get_num_cpus() << "\n" << "Processes: " << monitor.get_process_monitor().process_count() << "\n";
    return false;
}