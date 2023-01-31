#include "vga.h"
#include "common.h"

typedef struct {
    u8 character;
    u8 color;
} Char;

typedef struct {
    Char* buffer;
    usize column;
    usize row;
    u8 color;
} VGA;

#define MAKE_COLOR(foreground, background)                                     \
    (Color)((foreground) | (background) << 4)

const static usize max_columns = 80;
const static usize max_rows = 25;

VGA global_vga = {
    .buffer = (Char*)0xb8000,
    .column = 0,
    .row = 0,
    .color = MAKE_COLOR(ColorWhite, ColorBlack),
};

static inline void clear_row(usize row)
{
    Char empty_char = {
        .character = ' ',
        .color = global_vga.color,
    };
    for (usize x = 0; x < max_columns; ++x)
        global_vga.buffer[x + row * max_columns] = empty_char;
}

void vga_clear(void)
{
    for (usize row = 0; row < max_rows; ++row)
        clear_row(row);
}

void vga_linefeed(void)
{
    global_vga.column = 0;
    if (global_vga.row < max_rows - 1) {
        ++global_vga.row;
        return;
    }
    for (usize y = 1; y < max_rows; ++y)
        for (usize x = 0; x < max_columns; ++x)
            global_vga.buffer[x + (y - 1) * max_columns]
                = global_vga.buffer[x + y * max_columns];
    clear_row(max_rows - 1);
}

void vga_put_char(char value)
{
    if (value == '\n') {
        vga_linefeed();
        return;
    }
    if (global_vga.column > max_columns)
        vga_linefeed();
    global_vga.buffer[global_vga.column + global_vga.row * max_columns]
        = (Char) {
              .character = (u8)value,
              .color = global_vga.color,
          };
    ++global_vga.column;
}

void vga_put_string(const char* value)
{
    for (usize i = 0; value[i] != '\0'; ++i)
        vga_put_char(value[i]);
}

void vga_set_color(Color foreground, Color background)
{
    global_vga.color = MAKE_COLOR(foreground, background);
}

void vga_set_position(usize row, usize column)
{
    global_vga.row = row;
    global_vga.column = column;
}
