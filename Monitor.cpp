#include "Monitor.hpp"
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iomanip>
#include <cstring>

HistoryBuffer::HistoryBuffer(size_t max_size) : max_size_(max_size) {}

void HistoryBuffer::add_snapshot(const SystemSnapshot& snapshot) {
    buffer_.push_back(snapshot);
    if (buffer_.size() > max_size_) buffer_.pop_front();
}

std::vector<SystemSnapshot> HistoryBuffer::get_history() const {
    return {buffer_.begin(), buffer_.end()};
}

SystemSnapshot HistoryBuffer::get_latest() const {
    return buffer_.empty() ? SystemSnapshot{} : buffer_.back();
}

size_t HistoryBuffer::size() const { return buffer_.size(); }
void HistoryBuffer::clear() { buffer_.clear(); }

double HistoryBuffer::get_avg_cpu(size_t samples) const {
    if (buffer_.empty()) return 0.0;
    auto start = buffer_.size() > samples ? buffer_.end() - samples : buffer_.begin();
    double sum = std::accumulate(start, buffer_.end(), 0.0, [](double acc, const SystemSnapshot& s){ return acc + s.cpu_usage_percent; });
    return sum / std::distance(start, buffer_.end());
}

double HistoryBuffer::get_max_cpu(size_t samples) const {
    if (buffer_.empty()) return 0.0;
    auto start = buffer_.size() > samples ? buffer_.end() - samples : buffer_.begin();
    return std::max_element(start, buffer_.end(), [](const auto& a, const auto& b){ return a.cpu_usage_percent < b.cpu_usage_percent; })->cpu_usage_percent;
}

double HistoryBuffer::get_avg_mem(size_t samples) const {
    if (buffer_.empty()) return 0.0;
    auto start = buffer_.size() > samples ? buffer_.end() - samples : buffer_.begin();
    double sum = std::accumulate(start, buffer_.end(), 0.0, [](double acc, const SystemSnapshot& s){ return acc + s.mem_usage_percent; });
    return sum / std::distance(start, buffer_.end());
}

ProcessMonitor::ProcessMonitor() : first_update_(true) {
    std::memset(&prev_total_cpu_, 0, sizeof(prev_total_cpu_));
    last_update_ = std::chrono::system_clock::now();
}

void ProcessMonitor::update() {
    auto now = std::chrono::system_clock::now();
    cpu_stats_t curr_total_cpu;
    if (read_cpu_stats(&curr_total_cpu) != 0) return;
    int pids[4096], count;
    if (get_all_pids(pids, 4096, &count) != 0) return;
    processes_.clear();
    for (int i = 0; i < count; ++i) {
        ProcessSnapshot snap;
        if (read_process_info(pids[i], &snap.info) == 0) {
            snap.timestamp = now;
            processes_[pids[i]] = snap;
        }
    }
    if (!first_update_) calculate_cpu_percentages();
    prev_total_cpu_ = curr_total_cpu;
    last_update_ = now;
    first_update_ = false;
}

void ProcessMonitor::calculate_cpu_percentages() {
    cpu_stats_t curr_total;
    if (read_cpu_stats(&curr_total) != 0) return;
    uint64_t total_delta = curr_total.total - prev_total_cpu_.total;
    if (total_delta == 0) return;
    uint64_t total_mem = get_total_memory();
    int page_size = get_page_size();
    for (auto& [pid, snap] : processes_) {
        auto& proc = snap.info;
        proc.cpu_percent = 0.0; // pholder
        if (total_mem > 0) {
            proc.mem_percent = 100.0 * proc.rss * page_size / (total_mem * 1024);
        }
    }
}

std::vector<ProcessSnapshot> ProcessMonitor::get_top_processes_by_cpu(size_t count) const {
    std::vector<ProcessSnapshot> procs;
    procs.reserve(processes_.size());
    for (const auto& [_, snap] : processes_) procs.push_back(snap);
    std::partial_sort(procs.begin(), procs.begin() + std::min(count, procs.size()), procs.end(), [](const auto& a, const auto& b){ return a.info.cpu_percent > b.info.cpu_percent; });
    if (procs.size() > count) procs.resize(count);
    return procs;
}

std::vector<ProcessSnapshot> ProcessMonitor::get_top_processes_by_memory(size_t count) const {
    std::vector<ProcessSnapshot> procs;
    procs.reserve(processes_.size());
    for (const auto& [_, snap] : processes_) procs.push_back(snap);
    std::partial_sort(procs.begin(), procs.begin() + std::min(count, procs.size()), procs.end(), [](const auto& a, const auto& b){ return a.info.rss > b.info.rss; });
    if (procs.size() > count) procs.resize(count);
    return procs;
}

std::vector<ProcessSnapshot> ProcessMonitor::get_all_processes() const {
    std::vector<ProcessSnapshot> procs;
    procs.reserve(processes_.size());
    for (const auto& [_, snap] : processes_) procs.push_back(snap);
    return procs;
}

ProcessSnapshot ProcessMonitor::get_process(int pid) const {
    auto it = processes_.find(pid);
    return it != processes_.end() ? it->second : ProcessSnapshot{};
}

bool ProcessMonitor::process_exists(int pid) const {
    return processes_.count(pid) > 0;
}

size_t ProcessMonitor::process_count() const { return processes_.size(); }

SystemMonitor::SystemMonitor(size_t history_size) : history_(history_size), first_update_(true) {
    current_ = {};
    previous_ = {};
}

void SystemMonitor::update() {
    previous_ = current_;
    current_.timestamp = std::chrono::system_clock::now();
    read_cpu_stats(&current_.cpu);
    read_mem_stats(&current_.memory);
    disk_stats_t disks[16];
    int disk_count;
    if (read_disk_stats(disks, 16, &disk_count) == 0) {
        current_.disks.assign(disks, disks + disk_count);
    }
    net_stats_t nets[16];
    int net_count;
    if (read_net_stats(nets, 16, &net_count) == 0) {
        current_.networks.assign(nets, nets + net_count);
    }
    read_uptime(&current_.uptime);
    current_.cpu_usage_percent = first_update_ ? 0.0 : calculate_cpu_usage(&previous_.cpu, &current_.cpu);
    first_update_ = false;
    if (current_.memory.total > 0) {
        current_.mem_used_kb = current_.memory.total - current_.memory.available;
        current_.mem_usage_percent = 100.0 * current_.mem_used_kb / current_.memory.total;
    }
    history_.add_snapshot(current_);
    process_monitor_.update();
}

SystemSnapshot SystemMonitor::get_current_snapshot() const { return current_; }
HistoryBuffer& SystemMonitor::get_history() { return history_; }
ProcessMonitor& SystemMonitor::get_process_monitor() { return process_monitor_; }
double SystemMonitor::get_cpu_usage() const { return current_.cpu_usage_percent; }
double SystemMonitor::get_memory_usage() const { return current_.mem_usage_percent; }
uint64_t SystemMonitor::get_memory_used_mb() const { return current_.mem_used_kb / 1024; }
uint64_t SystemMonitor::get_memory_total_mb() const { return current_.memory.total / 1024; }

std::string SystemMonitor::format_uptime() const { return format_duration(current_.uptime.uptime_seconds); }

std::string SystemMonitor::format_memory() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << format_bytes(current_.mem_used_kb * 1024) << " / " << format_bytes(current_.memory.total * 1024) << " (" << current_.mem_usage_percent << "%)";
    return oss.str();
}

std::string format_bytes(uint64_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit = 0;
    double size = static_cast<double>(bytes);
    while (size >= 1024.0 && unit < 4) { size /= 1024.0; ++unit; }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << " " << units[unit];
    return oss.str();
}

std::string format_duration(double seconds) {
    int days = seconds / 86400;
    int hours = (static_cast<int>(seconds) % 86400) / 3600;
    int mins = (static_cast<int>(seconds) % 3600) / 60;
    std::ostringstream oss;
    if (days > 0) oss << days << "d ";
    oss << hours << "h " << mins << "m";
    return oss.str();
}

std::string format_percentage(double percent) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << percent << "%";
    return oss.str();
}