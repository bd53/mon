#include "Monitor.hpp"
#include <iostream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <csignal>
#include <termios.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>

std::atomic<bool> running{true};
termios orig_term;

struct UserInfo {
    std::string username;
    int uid;
    int gid;
    std::string home;
    std::string shell;
};

struct MenuItem {
    const char* title;
    bool (*render)(SystemMonitor&);
};

void enable_raw_mode() {
    termios raw = orig_term;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term);
}

void cls() {
    std::cout << "\033[2J\033[1;1H";
}

std::string blue(const std::string& text) {
    return "\033[44;37m" + text + "\033[0m";
}

void print_bar(double percent) {
    std::cout << "[";
    int filled = static_cast<int>(percent / 100.0 * 40);
    for (int i = 0; i < 40; i++) {
        std::cout << (i < filled ? "█" : "░");
    }
    std::cout << "]";
}

bool check_exit_key() {
    fd_set fds;
    struct timeval tv = {0, 0};
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0) {
        char c = getchar();
        if (c == 'q' || c == 'Q') return true;
        if (c == 27) {
            tv = {0, 10000};
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);
            if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0 && getchar() == '[') {
                getchar();
            } else {
                return true;
            }
        }
    }
    return false;
}

std::vector<UserInfo> get_system_users() {
    std::vector<UserInfo> users;
    std::ifstream passwd("/etc/passwd");
    std::string line;
    while (std::getline(passwd, line)) {
        std::istringstream iss(line);
        UserInfo user;
        std::string uid_str, gid_str, skip;
        std::getline(iss, user.username, ':');
        std::getline(iss, skip, ':');
        std::getline(iss, uid_str, ':');
        std::getline(iss, gid_str, ':');
        std::getline(iss, skip, ':');
        std::getline(iss, user.home, ':');
        std::getline(iss, user.shell);
        std::istringstream(uid_str) >> user.uid;
        std::istringstream(gid_str) >> user.gid;
        if (user.uid >= 1000 && user.shell != "/usr/sbin/nologin" && user.shell != "/bin/false" && user.shell != "/sbin/nologin" && user.username != "nobody") {
            users.push_back(user);
        }
    }
    std::sort(users.begin(), users.end(), [](const UserInfo& a, const UserInfo& b) {
        return a.uid < b.uid;
    });
    return users;
}

bool render_system(SystemMonitor& monitor) {
    auto snapshot = monitor.get_current_snapshot();
    std::cout << "Uptime: " << monitor.format_uptime() << "\n" << "CPUs: " << get_num_cpus() << "\n" << "Processes: " << monitor.get_process_monitor().process_count() << "\n";
    return false;
}

bool render_cpu(SystemMonitor& monitor) {
    auto snapshot = monitor.get_current_snapshot();
    auto& history = monitor.get_history();
    std::cout << "Current CPU: ";
    print_bar(snapshot.cpu_usage_percent);
    std::cout << " " << snapshot.cpu_usage_percent << "%\n\n" << "Avg (10s): " << history.get_avg_cpu(10) << "%\n" << "Peak (10s): " << history.get_max_cpu(10) << "%\n";
    return false;
}

bool render_memory(SystemMonitor& monitor) {
    auto snapshot = monitor.get_current_snapshot();
    std::cout << "Used: " << monitor.format_memory() << "\n" << "Memory: ";
    print_bar(snapshot.mem_usage_percent);
    std::cout << "  " << snapshot.mem_usage_percent << "%\n\n";
    if (snapshot.memory.swap_total > 0) {
        uint64_t swap_used = snapshot.memory.swap_total - snapshot.memory.swap_free;
        double swap_percent = (100.0 * swap_used) / snapshot.memory.swap_total;
        std::cout << "Swap: " << format_bytes(swap_used * 1024) << " / " << format_bytes(snapshot.memory.swap_total * 1024) << " (" << swap_percent << "%)\n";
    }
    return false;
}

bool render_processes(SystemMonitor& monitor) {
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

bool render_profile(SystemMonitor&) {
    auto users = get_system_users();
    std::cout << std::left << std::setw(20) << "USERNAME" << std::setw(8) << "UID" << std::setw(8) << "GID" << std::setw(30) << "HOME" << "SHELL\n" << "--------------------------------------------------------------------------------\n";
    for (const auto& user : users) {
        std::string home = user.home.length() > 29 ? "..." + user.home.substr(user.home.length() - 26) : user.home;
        std::string shell = user.shell.length() > 20 ? "..." + user.shell.substr(user.shell.length() - 17) : user.shell;
        std::cout << std::left << std::setw(20) << user.username << std::setw(8) << user.uid << std::setw(8) << user.gid << std::setw(30) << home << shell << "\n";
    }
    std::cout << "\nTotal users: " << users.size() << "\n";
    return false;
}

bool render_timezone(SystemMonitor&) {
    char link_target[256];
    std::string tz_name = "Unknown";
    ssize_t len = readlink("/etc/localtime", link_target, sizeof(link_target) - 1);
    if (len != -1) {
        link_target[len] = '\0';
        std::string path(link_target);
        size_t pos = path.find("/zoneinfo/");
        if (pos != std::string::npos) {
            tz_name = path.substr(pos + 10);
        }
    }
    time_t now = time(nullptr);
    struct tm* local_time = localtime(&now);
    char offset_buf[10], time_buf[100];
    strftime(offset_buf, sizeof(offset_buf), "%z", local_time);
    strftime(time_buf, sizeof(time_buf), "%A, %B %d, %Y %H:%M:%S", local_time);
    std::string tz_offset = offset_buf;
    if (tz_offset.length() == 5) {
        tz_offset = "UTC" + tz_offset.substr(0, 3) + ":" + tz_offset.substr(3, 2);
    }
    std::cout << "Current Timezone: " << tz_name << " (" << tz_offset << ")\n\n" << "Local Time: " << time_buf << "\n";
    return false;
}

bool render_packages(SystemMonitor&) {
    static int scroll_offset = 0;
    static std::vector<std::pair<std::string, std::string>> packages;
    static bool loaded = false;
    if (!loaded) {
        cls();
        std::cout << blue(" Additional packages ") << "\n\n" << "Loading packages... (this may take a moment)\n";
        FILE* pipe = popen("pacman -Q 2>/dev/null", "r");
        if (pipe) {
            char buffer[512];
            while (fgets(buffer, sizeof(buffer), pipe)) {
                std::string line(buffer);
                line.erase(line.find_last_not_of("\n\r") + 1);
                size_t space_pos = line.find(' ');
                if (space_pos != std::string::npos) {
                    packages.push_back({line.substr(0, space_pos), line.substr(space_pos + 1)});
                }
            }
            pclose(pipe);
        }
        loaded = true;
        scroll_offset = 0;
    }
    std::cout << std::left << std::setw(40) << "PACKAGE" << "VERSION\n" << "--------------------------------------------------------------------------------\n";
    const int max_display = 15;
    int end = std::min(scroll_offset + max_display, static_cast<int>(packages.size()));
    for (int i = scroll_offset; i < end; i++) {
        std::string name = packages[i].first.length() > 39 ? packages[i].first.substr(0, 36) + "..." : packages[i].first;
        std::string version = packages[i].second.length() > 35 ? packages[i].second.substr(0, 32) + "..." : packages[i].second;
        std::cout << std::left << std::setw(40) << name << version << "\n";
    }
    if (end < static_cast<int>(packages.size())) {
        std::cout << "\n... " << (packages.size() - end) << " more below\n";
    }
    if (scroll_offset > 0) {
        std::cout << "... " << scroll_offset << " packages above\n";
    }
    std::cout << "\nTotal packages: " << packages.size() << "\n";
    fd_set fds;
    struct timeval tv = {0, 200000};
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0) {
        char c = getchar();
        if (c == 'q' || c == 'Q') return true;
        if (c == 27) {
            tv = {0, 10000};
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);
            if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0 && getchar() == '[') {
                char d = getchar();
                if (d == 'A') scroll_offset = std::max(0, scroll_offset - 1);
                else if (d == 'B') scroll_offset = std::min(std::max(0, static_cast<int>(packages.size()) - max_display), scroll_offset + 1);
            } else {
                return true;
            }
        }
    }
    return false;
}

const MenuItem MENU_ITEMS[] = {
    {"System overview", render_system},
    {"CPU", render_cpu},
    {"Memory", render_memory},
    {"Top processes (by memory)", render_processes},
    {"Profile", render_profile},
    {"Additional packages", render_packages},
    {"Timezone", render_timezone}
};
const int MENU_COUNT = 7;

void draw_main_menu(int selected) {
    cls();
    std::cout << "Press ↑/↓ and ENTER for info\n\n";
    for (int i = 0; i < MENU_COUNT; i++) {
        std::string item = std::string(MENU_ITEMS[i].title) + std::string(33 - strlen(MENU_ITEMS[i].title), ' ') + "+";
        std::cout << (i == selected ? blue(" " + item + " ") : " " + item) << "\n";
    }
}

void run_detail_screen(const MenuItem& item, SystemMonitor& monitor) {
    bool is_packages = item.render == render_packages;
    while (running) {
        monitor.update();
        cls();
        std::cout << blue(" " + std::string(item.title) + " ") << "\n\n";
        bool should_exit = item.render(monitor);
        if (should_exit) return;
        std::cout << "\n" << (is_packages ? "Use ↑/↓ to scroll, ESC or q to return" : "Press ESC or q to return.") << "\n";
        if (is_packages) continue;
        for (int i = 0; i < 20; i++) {
            usleep(100000);
            if (check_exit_key()) return;
        }
    }
}

int main() {
    std::signal(SIGINT, [](int){ running = false; });
    std::signal(SIGTERM, [](int){ running = false; });
    tcgetattr(STDIN_FILENO, &orig_term);
    enable_raw_mode();
    SystemMonitor monitor(60);
    monitor.update();
    int selected = 0;
    while (running) {
        draw_main_menu(selected);
        char c = getchar();
        if (c == 27 && getchar() == '[') {
            char d = getchar();
            if (d == 'A') selected = (selected + MENU_COUNT - 1) % MENU_COUNT;
            if (d == 'B') selected = (selected + 1) % MENU_COUNT;
        }
        if (c == '\n' || c == '\r') {
            run_detail_screen(MENU_ITEMS[selected], monitor);
        }
    }
    disable_raw_mode();
    cls();
    std::cout << "Shutting down...\n";
    return 0;
}