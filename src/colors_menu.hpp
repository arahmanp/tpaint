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

    void moveSelectionUp();
    void moveSelectionDown();
    void moveSelectionRight();
    void moveSelectionLeft();

    void handleKey(int key);

    void print();
};