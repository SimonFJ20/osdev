#include "port.h"

asm(
    ".global read_port\n"
    "read_port:\n"
	"    mov 4(%esp), %edx\n"
	"    in %dx, %al\n"
	"    ret\n"
);

asm(
    ".global write_port\n"
    "write_port:\n"
	"    mov 4(%esp), %edx\n"
	"    mov 8(%esp), %al\n"
	"    out %al, %dx\n"
	"    ret\n"
);
