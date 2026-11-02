#include "styles.hpp"
#include <string>

std::string renderStyle(Style style) {
    switch (style) {
        case Style::Bold: return BOLD;
        case Style::Italic: return ITALIC;
        case Style::Underline: return UNDERLINE;
    }
}