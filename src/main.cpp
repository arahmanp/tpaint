#include "nbinput.hpp"
#include <iostream>

int main() {
    nbi::Input in;

    bool running = true;

    while(running) {
        int k = in.poll();
        if(k != nbi::NONE) {
            std::cout << "You pressed " << static_cast<char>(k) << '\n';
        }
    }
}