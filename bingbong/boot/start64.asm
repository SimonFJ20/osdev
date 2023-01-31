
extern kmain
global start64

section .text
bits 64

start64:
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call kmain

    cli
.halt:
    hlt
    jmp .halt
