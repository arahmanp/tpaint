#pragma once

#include <string>

const std::string FG_BLACK   = "\033[30m";
const std::string FG_RED     = "\033[31m";
const std::string FG_GREEN   = "\033[32m";
const std::string FG_YELLOW  = "\033[33m";
const std::string FG_BLUE    = "\033[34m";
const std::string FG_MAGENTA = "\033[35m";
const std::string FG_CYAN    = "\033[36m";
const std::string FG_WHITE   = "\033[37m";
const std::string FG_BRIGT_BLACK   = "\033[90m";
const std::string FG_BRIGT_RED     = "\033[91m";
const std::string FG_BRIGT_GREEN   = "\033[92m";
const std::string FG_BRIGT_YELLOW  = "\033[93m";
const std::string FG_BRIGT_BLUE    = "\033[94m";
const std::string FG_BRIGT_MAGENTA = "\033[95m";
const std::string FG_BRIGT_CYAN    = "\033[96m";
const std::string FG_BRIGT_WHITE   = "\033[97m";


const std::string BG_BLACK   = "\033[40m";
const std::string BG_RED     = "\033[41m";
const std::string BG_GREEN   = "\033[42m";
const std::string BG_YELLOW  = "\033[43m";
const std::string BG_BLUE    = "\033[44m";
const std::string BG_MAGENTA = "\033[45m";
const std::string BG_CYAN    = "\033[46m";
const std::string BG_WHITE   = "\033[47m";
const std::string BG_BRIGHT_BLACK   = "\033[100m";
const std::string BG_BRIGHT_RED     = "\033[101m";
const std::string BG_BRIGHT_GREEN   = "\033[102m";
const std::string BG_BRIGHT_YELLOW  = "\033[103m";
const std::string BG_BRIGHT_BLUE    = "\033[104m";
const std::string BG_BRIGHT_MAGENTA = "\033[105m";
const std::string BG_BRIGHT_CYAN    = "\033[106m";
const std::string BG_BRIGHT_WHITE   = "\033[107m";

const std::string RESET = "\033[0m";

enum class Color {
    Black             = 0,
    Red               = 1,
    Green             = 2,
    Yellow            = 3,
    Blue              = 4,
    Magenta           = 5,
    Cyan              = 6,
    White             = 7,
    BrightBlack       = 8,
    BrightRed         = 9,
    BrightGreen       = 10,
    BrightYellow      = 11,
    BrightBlue        = 12,
    BrightMagenta     = 13,
    BrightCyan        = 14,
    BrightWhite       = 715,
};

std::string renderBgColor(Color color);
