
global long_mode_start

section .text
bits 64

long_mode_start:
    .video_memory: equ 0xb8000

    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov dword [.video_memory], 0x2f4b2f4f ; print "OK"

    hlt
