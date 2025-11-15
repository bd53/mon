#ifndef RUNNER_HPP
#define RUNNER_HPP

#include "../Monitor.hpp"
#include "../ui/Menu.hpp"
#include <atomic>

class ScreenRunner {
    public:
        static void run_detail_screen(const MenuItem& item, SystemMonitor& monitor, std::atomic<bool>& running);
};

#endif