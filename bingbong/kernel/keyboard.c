#include "common.h"
#include "keyboard.h"
#include "vga.h"

static const u16 keyboard_data_port = 0x60;
static const u16 keyboard_status_port = 0x64;

u8 port_read(u16 port)
{
    u8 result;
    __asm__("mov dx, %[port]\t\n"
            "in %[result], dx\t\n"
            : [result] "=r"(result)
            : [port] "r"(port));
    return result;
}

void port_write(u16 port, u8 value)
{
    __asm__("mov dx, %[port]\t\n"
            "mov al, %[value]\t\n"
            "out dx, al\t\n"
            :
            : [port] "r"(port), [value] "r"(value));
}

void load_idt(void* address)
{
    __asm__("lidt [%[address]]\t\n"
            "sti\t\n"
            :
            : [address] "r"(address));
}

void keyboard_create(Keyboard* self)
{
    *self = (Keyboard) {
        .keys_pressed = { 0 },
        .key_has_been_released = false,
        .key_has_been_pressed = false,
        .active_char = '\0',
    };
}

char qwerty_key_char_indicator(u8 key)
{
    u8 char_indicator = key &= ~(1 << 7);
    // clang-format off
    switch (char_indicator) {
        case 0x2: return '1';  case 0x3: return '2';  case 0x4: return '3';
        case 0x5: return '4';  case 0x6: return '5';  case 0x7: return '6';
        case 0x8: return '7';  case 0x9: return '8';  case 0xa: return '9';
        case 0xb: return '0';  case 0xe: return '\b'; case 0x1c: return '\n';
        case 0x10: return 'q'; case 0x11: return 'w'; case 0x12: return 'e';
        case 0x13: return 'r'; case 0x14: return 't'; case 0x15: return 'y';
        case 0x16: return 'u'; case 0x17: return 'i'; case 0x18: return 'o';
        case 0x19: return 'p'; case 0x1e: return 'a'; case 0x1f: return 's';
        case 0x20: return 'd'; case 0x21: return 'f'; case 0x22: return 'g';
        case 0x23: return 'h'; case 0x24: return 'j'; case 0x25: return 'k';
        case 0x26: return 'l'; case 0x2c: return 'z'; case 0x2d: return 'x';
        case 0x2e: return 'c'; case 0x2f: return 'v'; case 0x30: return 'b';
        case 0x31: return 'n'; case 0x32: return 'm'; case 0x34: return '.';
        case 0x39: return ' '; default: return '\0';
    }
    // clang-format on
}

bool qwerty_key_is_pressed(u8 key) { return (key >> 7) == 0; }

void keyboard_update(Keyboard* self)
{
    u8 key = port_read(keyboard_data_port);
    char char_indicator = qwerty_key_char_indicator(key);
    if (self->active_char == char_indicator && !self->key_has_been_released) {
        return;
    }
    if (char_indicator != 'c')
        return;
    bool is_pressed = qwerty_key_is_pressed(key);
    usize ki = char_indicator >= 64 ? 1 : 0;
    char ka = char_indicator >= 64 ? (char_indicator - 64) : char_indicator;
    bool already_pressed = (self->keys_pressed[ki] | 1 << ka) > 0;
    if (is_pressed) {
        self->keys_pressed[ki] |= 1 >> ka;
        if (!already_pressed) {
            self->key_has_been_pressed = true;
            self->active_char = char_indicator;
        }
    } else {
        self->keys_pressed[ki] &= ~(1 >> ka);
        if (already_pressed) {
            self->key_has_been_released = true;
            self->active_char = char_indicator;
        }
    }
}

bool keyboard_is_char_pressed(Keyboard* self, char value)
{
    return value >= 64 ? self->keys_pressed[value - 64]
                       : self->keys_pressed[value];
}

bool keyboard_key_has_been_released(Keyboard* self)
{
    return self->key_has_been_released;
}

bool keyboard_key_has_been_pressed(Keyboard* self)
{
    return self->key_has_been_pressed;
}

char keyboard_active_char(Keyboard* self) { return self->active_char; }

void keyboard_clear_press(Keyboard* self)
{
    self->key_has_been_pressed = false;
    self->key_has_been_released = false;
}
