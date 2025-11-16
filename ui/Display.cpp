#include "Display.hpp"
#include <iostream>

void Display::print_bar(double percent) {
    std::cout << "[";
    int filled = static_cast<int>(percent / 100.0 * 40);
    for (int i = 0; i < 40; i++) {
        std::cout << (i < filled ? "█" : "░");
    }
    std::cout << "]";
}