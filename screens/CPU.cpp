#include "CPU.hpp"
#include "../ui/Display.hpp"
#include <iostream>

bool CpuScreen::render(SystemMonitor& monitor) {
    auto snapshot = monitor.get_current_snapshot();
    auto& history = monitor.get_history();
    std::cout << "Current CPU: ";
    Display::print_bar(snapshot.cpu_usage_percent);
    std::cout << " " << snapshot.cpu_usage_percent << "%\n\n" << "Avg (10s): " << history.get_avg_cpu(10) << "%\n" << "Peak (10s): " << history.get_max_cpu(10) << "%\n";
    return false;
}