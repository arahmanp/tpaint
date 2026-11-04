#include "tpaint.hpp"
#include "colors.hpp"
#include "mode.hpp"
#include "utils.hpp"
#include <iostream>

TPaint::TPaint() : mode(Mode::Normal) {}

void TPaint::handleKey(int k) {
    if(k == 'n') mode = Mode::Normal;
    else if(k == 'd') mode = Mode::Draw;
    else if(k == 'c') mode = Mode::ColorSelection;
    else {
        switch (mode) {
            case Mode::ColorSelection:
                menu.handleKey(k);
                break;

            case Mode::Normal:
            case Mode::Draw:
                canvas.handleKey(k, mode, menu.getSelectedColor());
                break;
        }
    }
}

void TPaint::render() const {
    moveCursorToHomePosition();

    menu.render();

    std::cout << '\n';

    canvas.render();

    std::cout << '\n';

    switch (mode) {
        case Mode::Normal:
            std::cout << BG_BLUE << " -NORMAL- \n";
            break;

        case Mode::Draw:
            std::cout << BG_RED << "  -DRAW-  \n";
            break;

        case Mode::ColorSelection:
            std::cout << BG_YELLOW << FG_BLACK << " -COLOR-  \n";
    }

    std::cout << RESET;
}