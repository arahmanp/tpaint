#pragma once

#include "mode.hpp"
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

    void handleKey(int k, Mode mode, std::string color);

    void print();
};