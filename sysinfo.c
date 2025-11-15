#include "sysinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>

#define PROC_STAT "/proc/stat"
#define PROC_MEMINFO "/proc/meminfo"
#define PROC_DISKSTATS "/proc/diskstats"
#define PROC_NET_DEV "/proc/net/dev"
#define PROC_UPTIME "/proc/uptime"

int read_cpu_stats(cpu_stats_t *stats) {
    FILE *fp = fopen(PROC_STAT, "r");
    if (!fp) return -1;
    char line[256];
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return -1;
    }
    memset(stats, 0, sizeof(cpu_stats_t));
    int parsed = sscanf(line, "cpu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu", &stats->user, &stats->nice, &stats->system, &stats->idle, &stats->iowait, &stats->irq, &stats->softirq, &stats->steal, &stats->guest, &stats->guest_nice);
    fclose(fp);
    if (parsed < 4) return -1;
    stats->total = stats->user + stats->nice + stats->system + stats->idle +
    stats->iowait + stats->irq + stats->softirq + stats->steal;
    return 0;
}

double calculate_cpu_usage(const cpu_stats_t *prev, const cpu_stats_t *curr) {
    uint64_t prev_idle = prev->idle + prev->iowait;
    uint64_t curr_idle = curr->idle + curr->iowait;
    uint64_t total_diff = curr->total - prev->total;
    uint64_t idle_diff = curr_idle - prev_idle;
    return total_diff ? 100.0 * (total_diff - idle_diff) / total_diff : 0.0;
}

int read_mem_stats(mem_stats_t *stats) {
    FILE *fp = fopen(PROC_MEMINFO, "r");
    if (!fp) return -1;
    memset(stats, 0, sizeof(mem_stats_t));
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        uint64_t val;
        if (sscanf(line, "MemTotal: %lu kB", &val) == 1) stats->total = val;
        else if (sscanf(line, "MemFree: %lu kB", &val) == 1) stats->free = val;
        else if (sscanf(line, "MemAvailable: %lu kB", &val) == 1) stats->available = val;
        else if (sscanf(line, "Buffers: %lu kB", &val) == 1) stats->buffers = val;
        else if (sscanf(line, "Cached: %lu kB", &val) == 1) stats->cached = val;
        else if (sscanf(line, "SwapTotal: %lu kB", &val) == 1) stats->swap_total = val;
        else if (sscanf(line, "SwapFree: %lu kB", &val) == 1) stats->swap_free = val;
    }
    fclose(fp);
    return 0;
}

uint64_t get_total_memory(void) {
    mem_stats_t stats;
    return read_mem_stats(&stats) == 0 ? stats.total : 0;
}

int read_disk_stats(disk_stats_t *stats, int max_disks, int *count) {
    FILE *fp = fopen(PROC_DISKSTATS, "r");
    if (!fp) return -1;
    *count = 0;
    char line[512];
    while (fgets(line, sizeof(line), fp) && *count < max_disks) {
        unsigned int major, minor;
        disk_stats_t *ds = &stats[*count];
        int parsed = sscanf(line, "%u %u %s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu", &major, &minor, ds->device, &ds->reads_completed, &ds->reads_merged, &ds->sectors_read, &ds->time_reading, &ds->writes_completed, &ds->writes_merged, &ds->sectors_written, &ds->time_writing, &ds->io_in_progress, &ds->time_io, &ds->weighted_time_io);
        if (parsed >= 14) {
            if ((strncmp(ds->device, "sd", 2) == 0 && strlen(ds->device) == 3) || (strncmp(ds->device, "nvme", 4) == 0 && strstr(ds->device, "n") != NULL) || (strncmp(ds->device, "vd", 2) == 0 && strlen(ds->device) == 3)) {
                (*count)++;
            }
        }
    }
    fclose(fp);
    return 0;
}

int read_net_stats(net_stats_t *stats, int max_interfaces, int *count) {
    FILE *fp = fopen(PROC_NET_DEV, "r");
    if (!fp) return -1;
    *count = 0;
    char line[512];
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp) && *count < max_interfaces) {
        net_stats_t *ns = &stats[*count];
        char *colon = strchr(line, ':');
        if (!colon) continue;
        *colon = ' ';
        int parsed = sscanf(line, "%s %lu %lu %lu %lu %*u %*u %*u %*u %lu %lu %lu %lu", ns->interface, &ns->rx_bytes, &ns->rx_packets, &ns->rx_errors, &ns->rx_dropped, &ns->tx_bytes, &ns->tx_packets, &ns->tx_errors, &ns->tx_dropped);
        if (parsed >= 9 && strcmp(ns->interface, "lo") != 0) (*count)++;
    }
    fclose(fp);
    return 0;
}

int read_process_info(int pid, process_info_t *info) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    FILE *fp = fopen(path, "r");
    if (!fp) return -1;
    memset(info, 0, sizeof(process_info_t));
    info->pid = pid;
    char comm[256];
    int parsed = fscanf(fp, "%d %s %c %d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %ld %ld", &info->pid, comm, &info->state, &info->ppid, &info->utime, &info->stime, &info->priority, &info->nice);
    fclose(fp);
    if (parsed < 8) return -1;
    if (comm[0] == '(') {
        size_t len = strlen(comm);
        if (len > 2 && comm[len-1] == ')') {
            size_t copy_len = (len - 2 < sizeof(info->name)-1) ? len-2 : sizeof(info->name)-1;
            memcpy(info->name, comm + 1, copy_len);
            info->name[copy_len] = '\0';
        } else {
            info->name[0] = '\0';
        }
    } else {
        strncpy(info->name, comm, sizeof(info->name)-1);
        info->name[sizeof(info->name)-1] = '\0';
    }
    snprintf(path, sizeof(path), "/proc/%d/statm", pid);
    if ((fp = fopen(path, "r"))) {
        fscanf(fp, "%lu %lu", &info->vsize, &info->rss);
        fclose(fp);
    }
    return 0;
}

int read_uptime(uptime_info_t *uptime) {
    FILE *fp = fopen(PROC_UPTIME, "r");
    if (!fp) return -1;
    int parsed = fscanf(fp, "%lf %lf", &uptime->uptime_seconds, &uptime->idle_seconds);
    fclose(fp);
    return parsed == 2 ? 0 : -1;
}

int get_all_pids(int *pids, int max_pids, int *count) {
    DIR *dir = opendir("/proc");
    if (!dir) return -1;
    *count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) && *count < max_pids) {
        if (!isdigit(entry->d_name[0])) continue;
        int pid = atoi(entry->d_name);
        if (pid > 0) pids[(*count)++] = pid;
    }
    closedir(dir);
    return 0;
}

int get_page_size(void) { return sysconf(_SC_PAGESIZE); }
int get_num_cpus(void) { return sysconf(_SC_NPROCESSORS_ONLN); }
