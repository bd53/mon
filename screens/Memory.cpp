#include "Memory.hpp"
#include "../ui/Display.hpp"
#include <iostream>

extern "C" {
    #include "../sysinfo.h"
}

bool MemoryScreen::render(SystemMonitor& monitor) {
    auto snapshot = monitor.get_current_snapshot();
    std::cout << "Used: " << monitor.format_memory() << "\n" << "Memory: ";
    Display::print_bar(snapshot.mem_usage_percent);
    std::cout << "  " << snapshot.mem_usage_percent << "%\n\n";
    if (snapshot.memory.swap_total > 0) {
        uint64_t swap_used = snapshot.memory.swap_total - snapshot.memory.swap_free;
        double swap_percent = (100.0 * swap_used) / snapshot.memory.swap_total;
        std::cout << "Swap: " << format_bytes(swap_used * 1024) << " / " << format_bytes(snapshot.memory.swap_total * 1024) << " (" << swap_percent << "%)\n";
    }
    return false;
}
