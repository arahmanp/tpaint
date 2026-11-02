#include "canvas.hpp"
#include "colors.hpp"
#include <iostream>
#include <string>
#include <vector>

Canvas::Canvas() {
    row = 20;
    col = 20;

    focusX = 0;
    focusY = 0;

    canvas.assign(row, std::vector<std::string>(col, BG_WHITE + " " + RESET));
}

void Canvas::print() {
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            std::cout << canvas[i][j];
        }
        std::cout << '\n';
    }
}