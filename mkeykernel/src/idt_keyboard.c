#include "idt_keyboard.h"
#include "port.h"

asm(
    "load_idt:\n"
    "    mov 4(%esp), %edx\n"
    "    lidt (%edx)\n"
    "    sti\n"
    "    ret\n");

void idt_init(struct IDT_entry* IDT)
{
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    // populate IDT entry of keyboard's interrupt
    keyboard_address = (unsigned long) kb_event_interrupt;
    IDT[PIC1_DATA] = (struct IDT_entry) {
        .offset_lowerbits = keyboard_address & 0xffff,
        .selector = KERNEL_CODE_SEGMENT_OFFSET,
        .zero = 0,
        .type_attr = INTERRUPT_GATE,
        .offset_higherbits = (keyboard_address & 0xffff0000) >> 16
    };

    // ICW1 - begin initialization
    write_port(PIC1_COMMAND, 0x11);
    write_port(PIC2_COMMAND, 0x11);

    // ICW2 - remap offset address of IDT
    //   In x86 protected mode, we have to remap the PICs beyond PIC1_COMMAND because
    //   Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
    write_port(PIC1_DATA, PIC1_COMMAND);
    write_port(PIC2_DATA, 0x28);

    // ICW3 - setup cascading
    write_port(PIC1_DATA, 0x00);
    write_port(PIC2_DATA, 0x00);

    // ICW4 - environment info
    write_port(PIC1_DATA, 0x01);
    write_port(PIC2_DATA, 0x01);
    // Initialization finished

    // mask interrupts
    write_port(PIC1_DATA, 0xff);
    write_port(PIC2_DATA, 0xff);

    // fill the IDT descriptor
    idt_address = (unsigned long)IDT;
    idt_ptr[0] = (sizeof(struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;

    load_idt(idt_ptr);
}

struct KBEventHandler* kbEventHandler;

void kb_init(struct KBEventHandler* handler)
{
    kbEventHandler = handler;
    write_port(PIC1_DATA, KB_ENABLE_ONLY_IRQ1_FLAGS);
}

asm(
    "kb_event_interrupt:\n"
    "    call kb_event_interrupt_handler\n"
    "    iret\n");

void kb_event_interrupt_handler(void)
{
    unsigned char status;
    char keycode;

    // write EOI
    write_port(PIC1_COMMAND, PIC1_COMMAND);

    status = read_port(KB_STATUS_PORT);
    // Lowest bit of status will be set if buffer is not empty
    if (status & 0x01) {
        keycode = read_port(KB_DATA_PORT);
        if (keycode < 0)
            return;
        kbEventHandler->handler(keycode);
    }
}
