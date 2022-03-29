#include "util.h"
#include <stdint.h>

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

char* strncpy(char* dest, const char* src, size_t n)
{
    for (size_t i = 0; i < n; i++)
        dest[i] = '\0';
    for (size_t i = 0; src[i]; i++)
        dest[i] = src[i];
    return dest;
}

void i32_dec_str(char* result, const int32_t value)
{
    static const int power_of_tens[] = {
        1,
        10,
        100,
        1000,
        10000,
        100000,
        1000000,
        10000000,
        100000000,
        1000000000,
    };
    int digits[10] = { 0 };
    int remaining = value < 0 ? value * -1 : value;
    for (int i = 0; i < 10; i++) {
        digits[i] = remaining / power_of_tens[9 - i];
        remaining %= power_of_tens[9 - i];
    }
    size_t resulti = 0;
    if (value < 0)
        result[resulti++] = '-';
    bool hasdigit = false;
    for (int i = 0; i < 10; i++) {
        if (!(digits[i] == 0 && !hasdigit)) {
            result[resulti++] = digits[i] + 48;
            hasdigit = true;
        }
    }
    if (!hasdigit)
        result[resulti++] = '0';
    result[resulti] = '\0';
}

void u32_hex_str(char* result, const uint32_t value)
{
    static const int power_of_sixteen[] = {
        0x1,
        0x10,
        0x100,
        0x1000,
        0x10000,
        0x100000,
        0x1000000,
        0x10000000,
    };
    int digits[8] = { 0 };
    int remaining = value;
    for (int i = 0; i < 8; i++) {
        digits[i] = remaining / power_of_sixteen[7 - i];
        remaining %= power_of_sixteen[7 - i];
    }
    size_t resulti = 0;
    bool hasdigit = false;
    for (int i = 0; i < 8; i++) {
        if (!(digits[i] == 0 && !hasdigit)) {
            if (digits[i] >= 0x0 && digits[i] <= 0x9)
                result[resulti++] = digits[i] + 48;
            else if (digits[i] >= 0xA && digits[i] <= 0xF)
                result[resulti++] = digits[i] + 55;
            hasdigit = true;
        }
    }
    if (!hasdigit)
        result[resulti++] = '0';
    result[resulti] = '\0';
}
