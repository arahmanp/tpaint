#pragma once

#include <string>
#include <vector>

class Canvas {
private:
    int row = 20;
    int col = 100;

    std::vector<std::vector<std::string>> canvas;

    int focusX = 0;
    int focusY = 0;

public:
    Canvas();

    void print();
};