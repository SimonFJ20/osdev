#include "print.h"
#include "common.h"

typedef struct {
    u8 character;
    u8 color;
} Char;

#define MAKE_COLOR(foreground, background) ((foreground) | (background) << 4)

const static usize max_columns = 80;
const static usize max_rows = 25;

Char* video_buffer = (Char*)0xb8000;
usize current_column = 0;
usize current_row = 0;
u8 current_color = MAKE_COLOR(ColorWhite, ColorBlack);

static inline void clear_row(usize row)
{
    Char empty_char = {
        .character = ' ',
        .color = current_color,
    };
    for (usize x = 0; x < max_columns; ++x)
        video_buffer[x + row * max_columns] = empty_char;
}

void print_clear()
{
    Char empty_char = {
        .character = ' ',
        .color = current_color,
    };
    for (usize row = 0; row < max_rows; ++row)
        clear_row(row);
}

void print_linefeed()
{
    current_column = 0;
    if (current_row < max_rows - 1) {
        ++current_row;
        return;
    }
    for (usize y = 1; y < max_rows; ++y)
        for (isize x = 0; x < max_columns; ++x)
            video_buffer[x + (y - 1) * max_columns]
                = video_buffer[x + y * max_columns];
    clear_row(max_rows - 1);
}

void print_char(char value)
{
    if (value == '\n') {
        print_linefeed();
        return;
    }
    if (current_column > max_columns)
        print_linefeed();
    video_buffer[current_column + current_row * max_columns] = (Char) {
        .character = value,
        .color = current_color,
    };
    ++current_column;
}

void print_string(const char* value)
{
    for (usize i = 0; value[i] != '\0'; ++i)
        print_char(value[i]);
}

void print_set_color(Color foreground, Color background)
{
    current_color = MAKE_COLOR(foreground, background);
}
