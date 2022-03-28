bits 32

section .text

global pin
global pout
global load_idt
global keyboard_handler

extern handle_keyboard_interrupt

pin:
    mov edx, [esp + 4]
    in al, dx
    ret

pout:
    mov edx, [esp + 4]
    mov eax, [esp + 8]
    out dx, al
    ret

load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret

keyboard_handler:
    call handle_keyboard_interrupt
    iretd
