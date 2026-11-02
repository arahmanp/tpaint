#pragma once

#include <string>

const std::string BOLD        = "\033[1m";
const std::string ITALIC      = "\033[3m";
const std::string UNDERLINE   = "\033[4m";

enum class Style {
    Bold        = 0,
    Italic      = 1,
    Underline   = 2,
};

std::string renderStyle(Style style);