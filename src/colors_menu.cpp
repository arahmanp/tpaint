#include "colors_menu.hpp"
#include "colors.hpp"
#include "styles.hpp"
#include "nbinput.hpp"
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>

ColorsMenu::ColorsMenu() {
    row = 2;
    col = 4;

    selectedR = 0;
    selectedC = 0;

    options = {
        {
            "Black    " + BG_BLACK + "    " + RESET,
            "Red      " + BG_RED + "    " + RESET,
            "Green    " + BG_GREEN + "    " + RESET,
            "Yellow   " + BG_YELLOW + "    " + RESET,
        },
        {
            "Blue     " + BG_BLUE + "    " + RESET,
            "Magenta  " + BG_MAGENTA + "    " + RESET,
            "Cyan     " + BG_CYAN + "    " + RESET,
            "White    " + BG_WHITE + "    " + RESET,
        }
    };

    colors = {
        {BG_BLACK, BG_RED, BG_GREEN, BG_YELLOW},
        {BG_BLUE, BG_MAGENTA, BG_CYAN, BG_WHITE}
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

std::string ColorsMenu::getSelectedColor() const {
    return colors[selectedR][selectedC];
}

void ColorsMenu::print() const {
    std::cout << std::left;

    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            std::string currentOpt = options[i][j];

            // TODO: observe this later
            if(i == selectedR && j == selectedC) {
                std::cout << BOLD << UNDERLINE << ITALIC << FG_CYAN
                          << std::setw(30) << currentOpt
                          << RESET;
            } else {
                std::cout << std::setw(30) << currentOpt << RESET;
            }
        }

        std::cout << "\n\n";
    }

    std::cout << std::right;
}