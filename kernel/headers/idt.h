#ifndef IDT_H 
#include "types.h"
#define IDT_H
void InitIDT();
void SetIDTDescriptor(uint8_t Number, uint32_t Base, uint16_t Selector, uint8_t Flags);
static inline void outb(uint16_t Port, uint8_t Data);
static inline void inb(uint16_t Port);
#endif
