#include "Process.hpp"
#include <iostream>
#include <iomanip>

extern "C" {
    #include "../sysinfo.h"
}

bool ProcessScreen::render(SystemMonitor& monitor) {
    auto top = monitor.get_process_monitor().get_top_processes_by_memory(10);
    int page_size = get_page_size();
    std::cout << std::left << std::setw(7) << "PID" << std::setw(25) << "NAME" << std::setw(10) << "MEM%" << std::setw(15) << "RSS" << std::setw(8) << "STATE" << "\n" << "-----------------------------------------------------------\n";
    for (auto& p : top) {
        std::string name_str(p.info.name);
        std::string name = name_str.length() > 24 ? name_str.substr(0, 21) + "..." : name_str;
        uint64_t mem_bytes = p.info.rss * page_size;
        std::cout << std::left << std::setw(7) << p.info.pid << std::setw(25) << name << std::setw(10) << p.info.mem_percent << std::setw(15) << format_bytes(mem_bytes) << std::setw(8) << p.info.state << "\n";
    }
    return false;
}
