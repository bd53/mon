#ifndef CPU_HPP
#define CPU_HPP

#include "../Monitor.hpp"

class CpuScreen {
    public:
        static bool render(SystemMonitor& monitor);
};

#endif