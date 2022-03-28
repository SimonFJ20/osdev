#include "keyboard.h"
#include "keyboard_map.h"
#include "vga.h"
#include <stdint.h>

#define IDT_SIZE 256
#define KERNEL_CODE_SEGMENT_OFFSET 0x8
#define IDT_INTERRUPT_GATE_32BIT 0x8e
#define PCI1_COMMAND_PORT 0x20
#define PCI1_DATA_PORT 0x21
#define PCI2_COMMAND_PORT 0xA0
#define PCI2_DATA_PORT 0xA1
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

struct IDT_ptr {
    uint16_t limit;
    struct IDT_entry* base;
} __attribute__((packed));

struct IDT_entry {
    uint16_t offset_lowerbits;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_upperbits;
} __attribute__((packed));

extern char pin(uint16_t port);
extern void pout(uint16_t port, uint8_t data);
extern void load_idt(struct IDT_ptr* ptr);
extern void keyboard_handler();

struct IDT_entry IDT[IDT_SIZE];

struct term* global_term;

void keyboard_init(struct term* t)
{
    global_term = t;
    keyboard_init_idt();
    pout(PCI1_DATA_PORT, 0xFD);
    term_writestr(t, "keyboard initialized\n");
}

void keyboard_init_idt()
{
    uint32_t offset = (uint32_t)keyboard_handler;
    IDT[0x21] = (struct IDT_entry) {
        .offset_lowerbits = offset & 0x0000FFFF,
        .selector = KERNEL_CODE_SEGMENT_OFFSET,
        .zero = 0,
        .type_attr = IDT_INTERRUPT_GATE_32BIT,
        .offset_upperbits = (offset & 0xFFFF0000) >> 16,
    };
    pout(PCI1_COMMAND_PORT, 0x11);
    pout(PCI2_COMMAND_PORT, 0x11);

    pout(PCI1_DATA_PORT, 0x20);
    pout(PCI2_DATA_PORT, 0x28);

    pout(PCI1_DATA_PORT, 0x0);
    pout(PCI2_DATA_PORT, 0x0);

    pout(PCI1_DATA_PORT, 0x1);
    pout(PCI2_DATA_PORT, 0x1);

    pout(PCI1_DATA_PORT, 0xFF);
    pout(PCI2_DATA_PORT, 0xFF);

    struct IDT_ptr idt_ptr = {
        .limit = (sizeof (struct IDT_entry) * IDT_SIZE) - 1,
        .base = IDT,
    };
    load_idt(&idt_ptr);
}

void handle_keyboard_interrupt()
{
    term_writestr(global_term, "key struck\n");
    pout(PCI1_COMMAND_PORT, 0x20);
    uint8_t status = pin(KEYBOARD_STATUS_PORT);
    if (status & 0x1) {
        int8_t keycode = pin(KEYBOARD_DATA_PORT);
        if (keycode < 0) return;
        term_putchar(global_term, keyboard_map[keycode]);
    }
}
