
section .multiboot_header
header_start:

    multiboot_magic_number equ 0xe85250d6
    i386_protected_mode equ 0
    checksum_base equ 0x100000000

    dd multiboot_magic_number
    dd 0
    dd header_end - header_start
    dd checksum_base - (multiboot_magic_number + 0 + (header_end - header_start))

    dw 0
    dw 0
    dw 8

header_end:
