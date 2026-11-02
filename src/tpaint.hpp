#pragma once

#include "canvas.hpp"
#include "colors_menu.hpp"
#include "mode.hpp"

class TPaint {
private:
    ColorsMenu menu;
    Canvas canvas;
    Mode mode;

public:
    TPaint();

    void handleKey(int k);

    void render() const;
};