
.section .text

.global read_port
read_port:
    mov 4(%esp), %edx
    in %dx, %al
    ret

.global write_port
write_port:
    mov 4(%esp), %edx
    mov 8(%esp), %al
    out %al, %dx
    ret

.global load_idt
load_idt:
    mov 4(%esp), %edx
    lidt (%edx)
    sti
    ret

.global keyboard_handler
keyboard_handler:
    call keyboard_handler_main
    iret
