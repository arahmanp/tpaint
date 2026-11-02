#include "canvas.hpp"
#include "colors.hpp"
#include "colors_menu.hpp"
#include "mode.hpp"
#include "nbinput.hpp"
#include "utils.hpp"
#include <iostream>

int main() {
    clearScreen();

    nbi::Input in;
    ColorsMenu menu;
    Canvas canvas;

    bool running = true;

    moveCursorToHomePosition();
    menu.print();

    std::cout << '\n';

    canvas.print();

    while(running) {
        if(in.pressed()) {
            int k = in.poll();

            if(k == 'q') running = false;

            menu.moveSelection(k);
            canvas.moveFocus(k, Mode::Draw, BG_BLUE);

            moveCursorToHomePosition();
            menu.print();

            std::cout << '\n';

            canvas.print();
        }
    }
}