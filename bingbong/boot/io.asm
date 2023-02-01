
global read_port
global write_port
global load_idt

section .text
bits 64

port_read:
    mov dx, di
    in al, di
    ret

port_write:
    mov dx, di
    mov al, sil
    out dx, al
    ret

load_idt:
    lidt [rdi]
    sti
    ret
