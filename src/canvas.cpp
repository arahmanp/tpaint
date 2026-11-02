#include "canvas.hpp"
#include "colors.hpp"
#include <iostream>
#include <string>
#include <vector>

Canvas::Canvas() :
    canvas(row, std::vector<std::string>(
        col,
        static_cast<std::string>(BG_WHITE) + " " + 
            static_cast<std::string>(RESET)
    )) {}

void Canvas::print() {
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            std::cout << canvas[i][j];
        }
        std::cout << '\n';
    }
}