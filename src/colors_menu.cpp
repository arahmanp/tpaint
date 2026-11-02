#include "colors_menu.hpp"
#include "colors.hpp"
#include "styles.hpp"
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>

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

void ColorsMenu::print() {
    std::cout << std::left;

    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            std::string currentOpt = options[i][j];

            // TODO: observe this later
            if(i == selectedR && j == selectedC) {
                std::cout << BOLD << UNDERLINE << ITALIC << FG_CYAN;
            }

            std::cout << std::setw(30) << currentOpt << RESET;
        }

        std::cout << "\n\n";
    }

    std::cout << std::right;
}