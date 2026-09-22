#ifndef IO_H
#define IO_H
#include "types.h"
inline uint8_t inb(uint16_t Port);
inline void outb(uint16_t Port, uint8_t Data);
#endif
