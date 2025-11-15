#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <string>
#include <termios.h>

class Terminal {
    public:
        static void init();
        static void cleanup();
        static void enable_raw_mode();
        static void disable_raw_mode();
        static void clear();
        static std::string blue(const std::string& text);
        static bool check_exit_key();

    private:
        static termios orig_term;
        static bool initialized;
};

#endif