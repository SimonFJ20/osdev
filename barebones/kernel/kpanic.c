#include "kernel.h"
#include "util.h"

void kpanic()
{
    const char* msg = "=== Kernel panic! ===";
    const int msg_len = 21;
    const int startposx = 80 / 2 - msg_len / 2;
    const int startposy = 2;

    for (int i = 0; i < msg_len; i++)
        *((uint16_t*)0xB8000 + (80 * startposy) + startposx + i) = (0xC0 << 8) ^ msg[i];

    for (;;) { }
}
