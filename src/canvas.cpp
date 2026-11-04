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

    canvas.assign(row, std::vector<Color>(col, Color::White));
}

void Canvas::handleKey(int k, Mode mode, Color color) {
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
        canvas[focusX][focusY] = color;
    }
}

void Canvas::render() const {
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            Color currentPixelColor = canvas[i][j];

            std::string pixel = renderBgColor(currentPixelColor);

            if(i == focusX && j == focusY) {
                std::string sign = BOLD;

                if(currentPixelColor == Color::Black) sign += FG_WHITE;
                else sign += FG_BLACK;

                sign += "+";

                pixel += sign + RESET;
            } else {
                pixel += " " + RESET;
            }

            std::cout << pixel;
        }
        std::cout << '\n';
    }
}