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

void vga_clear(void);
void vga_linefeed(void);
void vga_put_char(char value);
void vga_put_string(const char* value);
void vga_set_color(Color foreground, Color background);
void vga_set_position(usize row, usize column);
