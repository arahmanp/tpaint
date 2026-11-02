#pragma once

#include <string>
#include <vector>

class ColorsMenu {
private:
    int row;
    int col;

    int selectedR;
    int selectedC;

    std::vector<std::vector<std::string>> options;
    std::vector<std::vector<std::string>> colors;

public:
    ColorsMenu();

    std::string moveSelection(int k);

    std::string getSelectedColor() const;

    void print() const;
};