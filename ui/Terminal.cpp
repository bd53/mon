#include "Terminal.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/select.h>

termios Terminal::orig_term;
bool Terminal::initialized = false;

void Terminal::init() {
    if (!initialized) {
        tcgetattr(STDIN_FILENO, &orig_term);
        initialized = true;
    }
}

void Terminal::cleanup() {
    if (initialized) {
        disable_raw_mode();
    }
}

void Terminal::enable_raw_mode() {
    termios raw = orig_term;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void Terminal::disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term);
}

void Terminal::clear() {
    std::cout << "\033[2J\033[1;1H";
}

std::string Terminal::blue(const std::string& text) {
    return "\033[44;37m" + text + "\033[0m";
}

bool Terminal::check_exit_key() {
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