#ifndef IDT_H
#include <stdint.h>
#define IDT_H
void InitIDT();
void SetIDTDescriptor(uint8_t Number, void* ISR, uint8_t Flags);
#endif
