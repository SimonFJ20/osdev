#include <stddef.h>
#include <stdint.h>

#include "kernel.h"

const uint8_t PIC1_COMMAND = 0x20;
const uint8_t PIC2_COMMAND = 0xA0;
const uint8_t PIC1_DATA = 0x21;
const uint8_t PIC2_DATA = 0xA1;
const uint8_t INTERRUPT_GATE = 0x8e;
const uint8_t KERNEL_CODE_SEGMENT_OFFSET = 0x08;

const uint8_t KB_ENABLE_ONLY_IRQ1_FLAGS = 0xFD;
const uint8_t KB_DATA_PORT = 0x60;
const uint8_t KB_STATUS_PORT = 0x64;

char port_in(uint16_t port);
__asm__(
    ".global port_in\n"
    "port_in:\n"
    "    mov 4(%esp), %edx\n"
    "    in %dx, %al\n"
    "    ret\n");
    
void port_out(uint16_t port, uint8_t data);
__asm__(
    ".global port_out\n"
    "port_out:\n"
    "    mov 4(%esp), %edx\n"
    "    mov 8(%esp), %al\n"
    "    out %al, %dx\n"
    "    ret\n");

void idt_load(uint64_t* idtptr);
__asm__(
    "idt_load:\n"
    "    mov 4(%esp), %edx\n"
    "    lidt (%edx)\n"
    "    sti\n"
    "    ret\n");

void kb_event_interrupt();
__asm__(
    "kb_event_interrupt:\n"
    "    call kb_event_interrupt_handler\n"
    "    iret\n");

static inline void ICW1_begin_init()
{
    // ICW1 - begin initialization
    port_out(PIC1_COMMAND, 0x11);
    port_out(PIC2_COMMAND, 0x11);
}

static inline void ICW2_remap_offset_address()
{
    // ICW2 - remap offset address of IDT
    //   In x86 protected mode, we have to remap the PICs beyond PIC1_COMMAND because
    //   Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
    port_out(PIC1_DATA, 0x20);
    port_out(PIC2_DATA, 0x28);
}

static inline void ICW3_setup_cascading()
{
    // ICW3 - setup cascading
    port_out(PIC1_DATA, 0x00);
    port_out(PIC2_DATA, 0x00);
}

static inline void ICW4_env_info()
{
    // ICW4 - environment info
    port_out(PIC1_DATA, 0x01);
    port_out(PIC2_DATA, 0x01);
}

static inline void interrupts_mask()
{
    // mask interrupts
    port_out(PIC1_DATA, 0xff);
    port_out(PIC2_DATA, 0xff);
}

void idt_init(struct IDTEntry* idt)
{
    uint64_t keyboard_address;

    // populate IDT entry of keyboard's interrupt
    keyboard_address = (unsigned long)kb_event_interrupt;
    idt[PIC1_DATA] = (struct IDTEntry) {
        .offset_lowerbits = keyboard_address & 0xffff,
        .selector = KERNEL_CODE_SEGMENT_OFFSET,
        .zero = 0,
        .type_attr = INTERRUPT_GATE,
        .offset_higherbits = (keyboard_address & 0xffff0000) >> 16
    };

    ICW1_begin_init();
    ICW2_remap_offset_address();
    ICW3_setup_cascading();
    ICW4_env_info();
    // Initialization finished
    interrupts_mask();

    // fill the IDT descriptor
    uint64_t idt_ptr[2];
    idt_ptr[0] = (sizeof(struct IDTEntry) * IDT_SIZE) + (((unsigned long)idt & 0xffff) << 16);
    idt_ptr[1] = (unsigned long)idt >> 16;

    idt_load(idt_ptr);
}

struct KBEventHandler* kbEventHandler;

void kb_init(struct KBEventHandler* handler)
{
    kb_event_interrupt_handler();
    kbEventHandler = handler;
    port_out(PIC1_DATA, KB_ENABLE_ONLY_IRQ1_FLAGS);
}

void kb_event_interrupt_handler()
{
    uint8_t status;
    char keycode;

    // write EOI
    port_out(PIC1_COMMAND, PIC1_COMMAND);

    status = port_in(KB_STATUS_PORT);
    // Lowest bit of status will be set if buffer is not empty
    if (status & 0x01) {
        keycode = port_in(KB_DATA_PORT);
        if (keycode < 0)
            return;
        kbEventHandler->handler(keycode, &kbEventHandler->args);
    }
}
