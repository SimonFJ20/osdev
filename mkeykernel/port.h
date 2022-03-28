#ifndef PORT_H
#define PORT_H

char read_port(unsigned short port);
void write_port(unsigned short port, unsigned char data);

#endif