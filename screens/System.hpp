#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "../Monitor.hpp"

class SystemScreen {
    public:
        static bool render(SystemMonitor& monitor);
};

#endif