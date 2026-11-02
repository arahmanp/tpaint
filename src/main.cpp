#include "nbinput.hpp"
#include "tpaint.hpp"
#include "utils.hpp"

int main() {
    clearScreen();

    nbi::Input in;
    TPaint tpaint;

    bool running = true;

    tpaint.render();

    while(running) {
        if(in.pressed()) {
            int k = in.poll();

            if(k == 'q') running = false;

            tpaint.handleKey(k);

            tpaint.render();
        }
    }
}