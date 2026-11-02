#pragma once

#include <string>
#include <vector>

class Canvas {
private:
    int row;
    int col;

    int focusX;
    int focusY;

    std::vector<std::vector<std::string>> canvas;

public:
    Canvas();

    void print();
};