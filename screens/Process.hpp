#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "../Monitor.hpp"

class ProcessScreen {
    public:
        static bool render(SystemMonitor& monitor);
};

#endif