
global start
extern long_mode_start

section .text
bits 32

start:
    mov esp, stack_top
    
    call assert_multiboot
    call assert_cpuid
    call assert_long_mode

    call setup_page_tables
    call enable_paging

    lgdt [gdt64.pointer]
    jmp gdt64.code_segment: long_mode_start

assert_multiboot:
    multiboot_indicator equ 0x36d76289
    cmp eax, multiboot_indicator
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, 'M'
    jmp error

assert_cpuid:
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, 'C'
    jmp error

assert_long_mode:
    .has_extended_processor_info_indicator: equ 0x80000000
    .is_long_mode_available_indicator: equ 0x80000001

    mov eax, .has_extended_processor_info_indicator
    cpuid
    cmp eax, .has_extended_processor_info_indicator + 1
    jb .no_long_mode
    mov eax, .is_long_mode_available_indicator
    cpuid
    test edx, 1 << 29
    jz .no_long_mode
    ret
.no_long_mode:
    mov al, 'L'
    jmp error

setup_page_tables:
    .present_flag: equ 1 << 1
    .writable_flag: equ 1 << 0
    .huge_page_flag: equ 1 << 7
    
    mov eax, page_table_l3
    or eax, .present_flag | .writable_flag
    mov [page_table_l4], eax

    mov eax, page_table_l2
    or eax, .present_flag | .writable_flag
    mov [page_table_l3], eax

    mov ecx, 0
.loop:
    mov eax, 0x200000
    mul ecx
    or eax, .present_flag | .writable_flag | .huge_page_flag
    mov [page_table_l2 + ecx * 8], eax

    inc ecx
    cmp ecx, 512
    jne .loop
    ret

enable_paging:
    .enable_PAE_flag: equ 1 << 5
    .long_mode_indicator: equ 0xc0000080
    .long_mode_flag: equ 1 << 8
    .paging_flag: equ 1 << 31

    mov eax, page_table_l4
    mov cr3, eax
    mov eax, cr4

    or eax, .enable_PAE_flag
    mov cr4, eax
    mov ecx, .long_mode_indicator

    rdmsr
    or eax, .long_mode_flag
    wrmsr

    mov eax, cr0
    or eax, .paging_flag
    mov cr0, eax
    ret

error:
    .video_buffer: equ 0xb8000

    mov dword [.video_buffer], 0x4f524f45
    mov dword [.video_buffer + 4], 0x4f3a4f52
    mov dword [.video_buffer + 8], 0x4f204f20
    mov byte [.video_buffer + 10], al
    hlt

section .bss
align 0x1000

page_table_l4:
    resb 0x1000
page_table_l3:
    resb 0x1000
page_table_l2:
    resb 0x1000

stack_bottom:
    resb 0x1000
stack_top:

section .rodata
gdt64:
    .executable_flag: equ 1 << 43
    .code_and_data_flag: equ 1 << 44
    .present_flag: equ 1 << 47
    .mode_64_flag: equ 1 << 53

    dq 0
    .code_segment: equ $ - gdt64
    dq .executable_flag | .code_and_data_flag | .present_flag | .mode_64_flag
.pointer:
    dw $ - gdt64 - 1
    dq gdt64
