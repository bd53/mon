#include "Monitor.hpp"
#include <iostream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <csignal>
#include <termios.h>
#include <unistd.h>

std::atomic<bool> running{true};
termios orig_term;

enum Screen {
    MAIN_MENU,
    DETAIL_SYSTEM,
    DETAIL_CPU,
    DETAIL_MEMORY,
    DETAIL_PROCESSES
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
    const int width = 40;
    int filled = static_cast<int>(percent / 100.0 * width);
    std::cout << "[";
    for (int i = 0; i < width; i++) {
        std::cout << (i < filled ? "█" : "░");
    }
    std::cout << "]";
}

void detail_header(const std::string& title) {
    cls();
    std::cout << blue(" " + title + " ") << "\n\n";
}

bool check_exit_key() {
    if (isatty(STDIN_FILENO) && getchar() != EOF) {
        char c = std::cin.get();
        return (c == 27 || c == 'q');
    }
    return false;
}

void draw_main_menu(int selected) {
    cls();
    std::cout << "Press ↑/↓ and ENTER for info\n\n";
    const char* items[] = {
        "System overview                   +",
        "CPU                               +",
        "Memory                            +",
        "Top processes (by memory)         +"
    };
    for (int i = 0; i < 4; i++) {
        if (i == selected) {
            std::cout << blue(" " + std::string(items[i]) + " ") << "\n";
        } else {
            std::cout << " " << items[i] << "\n";
        }
    }
}

void screen_system(SystemMonitor& monitor) {
    detail_header("System overview");
    auto snapshot = monitor.get_current_snapshot();
    std::cout << "Uptime: " << monitor.format_uptime() << "\n" << "CPUs: " << get_num_cpus() << "\n" << "Processes: " << monitor.get_process_monitor().process_count() << "\n" << "\nPress ESC or q to return.\n";
}

void screen_cpu(SystemMonitor& monitor) {
    detail_header("CPU");
    auto snapshot = monitor.get_current_snapshot();
    auto& history = monitor.get_history();
    std::cout << "Current CPU: ";
    print_bar(snapshot.cpu_usage_percent);
    std::cout << " " << snapshot.cpu_usage_percent << "%\n\n" << "Avg (10s): " << history.get_avg_cpu(10) << "%\n" << "Peak (10s): " << history.get_max_cpu(10) << "%\n" << "\nPress ESC or q to return.\n";
}

void screen_memory(SystemMonitor& monitor) {
    detail_header("Memory");
    auto snapshot = monitor.get_current_snapshot();
    std::cout << "Used: " << monitor.format_memory() << "\n" << "Memory: ";
    print_bar(snapshot.mem_usage_percent);
    std::cout << "  " << snapshot.mem_usage_percent << "%\n\n";
    if (snapshot.memory.swap_total > 0) {
        uint64_t swap_used = snapshot.memory.swap_total - snapshot.memory.swap_free;
        double swap_percent = (100.0 * swap_used) / snapshot.memory.swap_total;
        std::cout << "Swap: " << format_bytes(swap_used * 1024) << " / " << format_bytes(snapshot.memory.swap_total * 1024) << " (" << swap_percent << "%)\n";
    }
    std::cout << "\nPress ESC or q to return.\n";
}

void screen_processes(SystemMonitor& monitor) {
    detail_header("Top processes (by memory)");
    auto top = monitor.get_process_monitor().get_top_processes_by_memory(10);
    int page_size = get_page_size();
    std::cout << std::left << std::setw(7) << "PID" << std::setw(25) << "NAME" << std::setw(10) << "MEM%" << std::setw(15) << "RSS" << std::setw(8) << "STATE" << "\n" << "-----------------------------------------------------------\n";
    for (auto& p : top) {
        std::string name = p.info.name;
        if (name.length() > 24) {
            name = name.substr(0, 21) + "...";
        }
        uint64_t mem_bytes = p.info.rss * page_size;
        std::cout << std::left << std::setw(7) << p.info.pid << std::setw(25) << name << std::setw(10) << p.info.mem_percent << std::setw(15) << format_bytes(mem_bytes) << std::setw(8) << p.info.state << "\n";
    }
    std::cout << "\nPress ESC or q to return.\n";
}

void run_detail(Screen screen, SystemMonitor& monitor) {
    while (running) {
        monitor.update();
        switch (screen) {
            case DETAIL_SYSTEM: screen_system(monitor); break;
            case DETAIL_CPU: screen_cpu(monitor); break;
            case DETAIL_MEMORY: screen_memory(monitor); break;
            case DETAIL_PROCESSES: screen_processes(monitor); break;
            default: break;
        }
        for (int i = 0; i < 20; i++) {
            usleep(100000);
            if (check_exit_key()) {
                return;
            }
        }
    }
}

Screen get_detail_screen(int selected) {
    const Screen screens[] = {
        DETAIL_SYSTEM,
        DETAIL_CPU,
        DETAIL_MEMORY,
        DETAIL_PROCESSES
    };
    return screens[selected];
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
            if (d == 'A') selected = (selected + 3) % 4;
            if (d == 'B') selected = (selected + 1) % 4;
        }
        if (c == '\n' || c == '\r') {
            run_detail(get_detail_screen(selected), monitor);
        }
    }
    disable_raw_mode();
    cls();
    std::cout << "Shutting down...\n";
    return 0;
}
