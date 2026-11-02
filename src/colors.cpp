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
    }
}