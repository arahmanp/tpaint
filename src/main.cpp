#include "canvas.hpp"
#include "colors_menu.hpp"
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

            menu.handleKey(k);

            moveCursorToHomePosition();
            menu.print();
            
            std::cout << '\n';

            canvas.print();
        }
    }
}