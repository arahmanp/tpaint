#include "colors.hpp"
#include <string>

std::string renderFgColor(Color color) {
    switch (color) {
        case Color::Black: return FG_BLACK;
        case Color::Red: return FG_RED;
        case Color::Green: return FG_GREEN;
        case Color::Yellow: return FG_YELLOW;
        case Color::Blue: return FG_BLUE;
        case Color::Magenta: return FG_MAGENTA;
        case Color::Cyan: return FG_CYAN;
        case Color::White: return FG_WHITE;
    }
}

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

std::string resetStyle() {
    return RESET;
}