#include "colors_menu.hpp"
#include "nbinput.hpp"
#include "utils.hpp"

int main() {
    clearScreen();

    nbi::Input in;
    ColorsMenu menu;

    bool running = true;

    moveCursorToHomePosition();
    menu.print();

    while(running) {
        if(in.pressed()) {
            int k = in.poll();

            menu.handleKey(k);

            moveCursorToHomePosition();
            menu.print();
        }
    }
}