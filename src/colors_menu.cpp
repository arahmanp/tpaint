#include "colors_menu.hpp"
#include "colors.hpp"
#include "styles.hpp"
#include "nbinput.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

ColorsMenu::ColorsMenu() {
    row = 2;
    col = 8;

    selectedR = 0;
    selectedC = 0;

    options = {
        {
            BG_BLACK + "    " + RESET,
            BG_RED + "    " + RESET,
            BG_GREEN + "    " + RESET,
            BG_YELLOW + "    " + RESET,
            BG_BLUE + "    " + RESET,
            BG_MAGENTA + "    " + RESET,
            BG_CYAN + "    " + RESET,
            BG_WHITE + "    " + RESET,
        },
        {
            BG_BRIGHT_BLACK + "    " + RESET,
            BG_BRIGHT_RED + "    " + RESET,
            BG_BRIGHT_GREEN + "    " + RESET,
            BG_BRIGHT_YELLOW + "    " + RESET,
            BG_BRIGHT_BLUE + "    " + RESET,
            BG_BRIGHT_MAGENTA + "    " + RESET,
            BG_BRIGHT_CYAN + "    " + RESET,
            BG_BRIGHT_WHITE + "    " + RESET,
        }
    };

    colors = {
        {
            Color::Black,
            Color::Red,
            Color::Green,
            Color::Yellow,
            Color::Blue,
            Color::Magenta,
            Color::Cyan,
            Color::White
        },
        {
            Color::BrightBlack,
            Color::BrightRed,
            Color::BrightGreen,
            Color::BrightYellow,
            Color::BrightBlue,
            Color::BrightMagenta,
            Color::BrightCyan,
            Color::BrightWhite
        },
    };
}

void ColorsMenu::handleKey(int k) {
    switch (k) {
        case nbi::Key::Up: 
            if(selectedR > 0) selectedR--;
            break;

        case nbi::Key::Down:
            if(selectedR < row - 1) selectedR++;
            break;

        case nbi::Key::Right:
            if(selectedC < col - 1) selectedC++;
            break;

        case nbi::Key::Left:
            if(selectedC > 0) selectedC--;
            break;
    }
}

Color ColorsMenu::getSelectedColor() const {
    return colors[selectedR][selectedC];
}

void ColorsMenu::render() const {
    std::cout << "Selected color: "
              << renderBgColor(getSelectedColor())
              << "    "
              << RESET;

    std::cout << "\n\n";

    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            std::string currentOpt = options[i][j];

            // TODO: observe this later
            if(i == selectedR && j == selectedC) {
                std::cout << BOLD << UNDERLINE;
                if(getSelectedColor() == Color::Black) std::cout << FG_WHITE;
                else std::cout << FG_BLACK;
                currentOpt = renderBgColor(getSelectedColor()) + " $$ " + RESET;
            }

            std::cout << currentOpt;
        }

        std::cout << '\n';
    }
}