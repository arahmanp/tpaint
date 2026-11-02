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

            switch (k) {
                case nbi::Key::Up: menu.moveSelectionUp(); break;
                case nbi::Key::Down: menu.moveSelectionDown(); break;
                case nbi::Key::Right: menu.moveSelectionRight(); break;
                case nbi::Key::Left: menu.moveSelectionLeft(); break;
                case 'q': running = false; break;
            }

            moveCursorToHomePosition();
            menu.print();
        }
    }
}