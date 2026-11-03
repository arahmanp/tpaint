#include "colors.hpp"
#include <string>

std::string renderBgColor(Color color) {
    switch (color) {
        case Color::Black: return BG_BLACK;
        case Color::Red: return BG_RED;
        case Color::Green: return BG_GREEN;
        case Color::Yellow: return BG_YELLOW;
        case Color::Blue: return BG_BLUE;
        case Color::Magenta: return BG_MAGENTA;
        case Color::Cyan: return BG_CYAN;
        case Color::White: return BG_WHITE;
        case Color::BrightBlack: return BG_BRIGHT_BLACK;
        case Color::BrightRed: return BG_BRIGHT_RED;
        case Color::BrightGreen: return BG_BRIGHT_GREEN;
        case Color::BrightYellow: return BG_BRIGHT_YELLOW;
        case Color::BrightBlue: return BG_BRIGHT_BLUE;
        case Color::BrightMagenta: return BG_BRIGHT_MAGENTA;
        case Color::BrightCyan: return BG_BRIGHT_CYAN;
        case Color::BrightWhite: return BG_BRIGHT_WHITE;
    }
}
