#pragma once

#include <string>
#include <vector>

class ColorsMenu {
private:
    std::vector<std::vector<std::string>> options = {
        {
            "Black (B):   \033[40m    \033[0m",
            "Red (R):     \033[41m    \033[0m",
            "Green (G):   \033[42m    \033[0m",
            "Yellow (Y):  \033[43m    \033[0m",
        },
        {
            "Blue (L):    \033[44m    \033[0m",
            "Magenta (M): \033[45m    \033[0m",
            "Cyan (C):    \033[46m    \033[0m",
            "White (W):   \033[47m    \033[0m",
        },
    };

    int row = 2;
    int col = 4;

    int selectedR = 0;
    int selectedC = 0;

public:
    void print();
};