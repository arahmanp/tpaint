#pragma once

#include "colors.hpp"
#include "mode.hpp"
#include <vector>

class Canvas {
private:
    int row;
    int col;

    int focusX;
    int focusY;

    std::vector<std::vector<Color>> canvas;

public:
    Canvas();

    void handleKey(int k, Mode mode, Color color);

    void print() const;
};