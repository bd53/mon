#ifndef SYSINFO_H
#define SYSINFO_H

#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        uint64_t user;
        uint64_t nice;
        uint64_t system;
        uint64_t idle;
        uint64_t iowait;
        uint64_t irq;
        uint64_t softirq;
        uint64_t steal;
        uint64_t guest;
        uint64_t guest_nice;
        uint64_t total;
    } cpu_stats_t;

    typedef struct {
        uint64_t total;
        uint64_t free;
        uint64_t available;
        uint64_t buffers;
        uint64_t cached;
        uint64_t swap_total;
        uint64_t swap_free;
    } mem_stats_t;

    typedef struct {
        char device[64];
        uint64_t reads_completed;
        uint64_t reads_merged;
        uint64_t sectors_read;
        uint64_t time_reading;
        uint64_t writes_completed;
        uint64_t writes_merged;
        uint64_t sectors_written;
        uint64_t time_writing;
        uint64_t io_in_progress;
        uint64_t time_io;
        uint64_t weighted_time_io;
    } disk_stats_t;

    typedef struct {
        char interface[32];
        uint64_t rx_bytes;
        uint64_t rx_packets;
        uint64_t rx_errors;
        uint64_t rx_dropped;
        uint64_t tx_bytes;
        uint64_t tx_packets;
        uint64_t tx_errors;
        uint64_t tx_dropped;
    } net_stats_t;

    typedef struct {
        int pid;
        char name[256];
        char state;
        int ppid;
        uint64_t utime;
        uint64_t stime;
        long priority;
        long nice;
        uint64_t vsize;
        uint64_t rss;
        unsigned int cpu_num;
        double cpu_percent;
        double mem_percent;
    } process_info_t;

    typedef struct {
        double uptime_seconds;
        double idle_seconds;
    } uptime_info_t;

    int read_cpu_stats(cpu_stats_t *stats);
    int read_mem_stats(mem_stats_t *stats);
    int read_disk_stats(disk_stats_t *stats, int max_disks, int *count);
    int read_net_stats(net_stats_t *stats, int max_interfaces, int *count);
    int read_process_info(int pid, process_info_t *info);
    int read_uptime(uptime_info_t *uptime);
    int get_all_pids(int *pids, int max_pids, int *count);
    double calculate_cpu_usage(const cpu_stats_t *prev, const cpu_stats_t *curr);
    int get_page_size(void);
    int get_num_cpus(void);
    uint64_t get_total_memory(void);
#ifdef __cplusplus
}
#endif

#endif