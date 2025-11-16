#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <chrono>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "sysinfo.h"

class HistoryBuffer;
class ProcessMonitor;

struct SystemSnapshot {
    cpu_stats_t cpu;
    mem_stats_t memory;
    std::vector<disk_stats_t> disks;
    std::vector<net_stats_t> networks;
    uptime_info_t uptime;
    std::chrono::system_clock::time_point timestamp;
    double cpu_usage_percent;
    uint64_t mem_used_kb;
    double mem_usage_percent;
};

struct ProcessSnapshot {
    process_info_t info;
    std::chrono::system_clock::time_point timestamp;
};

class HistoryBuffer {
    public:
        explicit HistoryBuffer(size_t max_size = 60);
        void add_snapshot(const SystemSnapshot& snapshot);
        std::vector<SystemSnapshot> get_history() const;
        SystemSnapshot get_latest() const;
        size_t size() const;
        void clear();
        double get_avg_cpu(size_t samples = 10) const;
        double get_max_cpu(size_t samples = 10) const;
        double get_avg_mem(size_t samples = 10) const;
    private:
        std::deque<SystemSnapshot> buffer_;
        size_t max_size_;
};

class ProcessMonitor {
    public:
        ProcessMonitor();
        void update();
        std::vector<ProcessSnapshot> get_top_processes_by_cpu(size_t count = 10) const;
        std::vector<ProcessSnapshot> get_top_processes_by_memory(size_t count = 10) const;
        std::vector<ProcessSnapshot> get_all_processes() const;
        ProcessSnapshot get_process(int pid) const;
        bool process_exists(int pid) const;
        size_t process_count() const;
    private:
        std::map<int, ProcessSnapshot> processes_;
        cpu_stats_t prev_total_cpu_;
        std::chrono::system_clock::time_point last_update_;
        bool first_update_;
        void calculate_cpu_percentages();
};

class SystemMonitor {
    public:
        SystemMonitor(size_t history_size = 60);
        void update();
        SystemSnapshot get_current_snapshot() const;
        HistoryBuffer& get_history();
        ProcessMonitor& get_process_monitor();
        double get_cpu_usage() const;
        double get_memory_usage() const;
        uint64_t get_memory_used_mb() const;
        uint64_t get_memory_total_mb() const;
        std::string format_uptime() const;
        std::string format_memory() const;
    private:
        SystemSnapshot current_;
        SystemSnapshot previous_;
        HistoryBuffer history_;
        ProcessMonitor process_monitor_;
        bool first_update_;
};

std::string format_bytes(uint64_t bytes);
std::string format_duration(double seconds);
std::string format_percentage(double percent);

#endif