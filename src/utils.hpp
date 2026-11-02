#pragma once

#include <iostream>

inline void clearScreen() {std::cout << "\033[2J";}
inline void moveCursorToHomePosition() {std::cout << "\033[H";}