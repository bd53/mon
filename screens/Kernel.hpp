#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "../Monitor.hpp"

class KernelScreen {
    public:
        static bool render(SystemMonitor& monitor);
};

#endif