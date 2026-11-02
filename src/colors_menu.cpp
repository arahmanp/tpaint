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

void ColorsMenu::moveSelectionUp() {
    if(selectedR > 0) selectedR--;
}

void ColorsMenu::moveSelectionDown() {
    if(selectedR < row - 1) selectedR++;
}

void ColorsMenu::moveSelectionRight() {
    if(selectedC < col - 1) selectedC++;
}

void ColorsMenu::moveSelectionLeft() {
    if(selectedC > 0) selectedC--;
}

void ColorsMenu::handleKey(int key) {
    switch (key) {
        case nbi::Key::Up: moveSelectionUp(); break;
        case nbi::Key::Down: moveSelectionDown(); break;
        case nbi::Key::Right: moveSelectionRight(); break;
        case nbi::Key::Left: moveSelectionLeft(); break;

        // ! temporary
        case 'q': exit(0);
    }
}

void ColorsMenu::print() {
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