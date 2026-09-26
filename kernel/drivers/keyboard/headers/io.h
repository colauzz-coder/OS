#ifndef IO_H
#define IO_H
#include <stdint.h>
uint8_t inb(uint16_t Port);
void outb(uint16_t Port, uint8_t Data);
#endif
