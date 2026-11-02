#include "canvas.hpp"
#include "colors.hpp"
#include "mode.hpp"
#include "nbinput.hpp"
#include "styles.hpp"
#include <iostream>
#include <string>
#include <vector>

Canvas::Canvas() {
    row = 20;
    col = 100;

    focusX = 0;
    focusY = 0;

    canvas.assign(row, std::vector<std::string>(col, BG_WHITE + " " + RESET));
}

void Canvas::handleKey(int k, Mode mode, std::string color) {
    switch (k) {
        case nbi::Key::Up:
            if(focusX > 0) focusX--;
            break;

        case nbi::Key::Down:
            if(focusX < row - 1) focusX++;
            break;

        case nbi::Key::Right:
            if(focusY < col - 1) focusY++;
            break;

        case nbi::Key::Left:
            if(focusY > 0) focusY--;
            break;
    }

    if(mode == Mode::Draw) {
        canvas[focusX][focusY] = color + " " + RESET;
    }
}

void Canvas::print() {
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            std::string currentPixel = canvas[i][j];

            if(i == focusX && j == focusY) {
                std::string bgFocus = currentPixel.substr(0, 5);
                std::string sign = BOLD;

                if(bgFocus == BG_BLACK) sign += FG_WHITE;
                else sign += FG_BLACK;

                sign += "+";

                currentPixel = bgFocus + sign + RESET;
            }

            std::cout << currentPixel;
        }
        std::cout << '\n';
    }
}