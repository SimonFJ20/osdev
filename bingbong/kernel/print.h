#pragma once

#include "common.h"

enum {
    ColorBlack,
    ColorBlue,
    ColorGreen,
    ColorCyan,
    ColorRed,
    ColorMagenta,
    ColorBrown,
    ColorLightGray,
    ColorDarkGray,
    ColorLightBlue,
    ColorLightGreen,
    ColorLightCyan,
    ColorLightRed,
    ColorPink,
    ColorYellow,
    ColorWhite,
};
typedef u8 Color;

void print_clear();
void print_linefeed();
void print_char(char value);
void print_string(const char* value);
void print_set_color(Color foreground, Color background);
