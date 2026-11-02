#pragma once

#include "colors.hpp"
#include <string>
#include <vector>

class ColorsMenu {
private:
    int row;
    int col;

    int selectedR;
    int selectedC;

    std::vector<std::vector<std::string>> options;
    std::vector<std::vector<Color>> colors;

public:
    ColorsMenu();

    void handleKey(int k);

    Color getSelectedColor() const;

    void print() const;
};