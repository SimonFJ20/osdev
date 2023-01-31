
global start

section .text

bits 32

start:
    
    video_memory_location equ 0xb8000
    OK_string equ 0x2f4b2f4f

    mov dword [video_memory_location], OK_string
    hlt
