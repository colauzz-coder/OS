#ifndef IO_H
#define IO_H
#include "../../../lib/types.h"
static inline uint8_t inb(uint16_t Port);
static inline void outb(uint16_t Port, uint8_t Data);
#endif
