#ifndef IDT_H 
#include "../../../lib/types.h"
#define IDT_H
void InitIDT();
void SetIDTDescriptor(uint8_t Number, uint32_t Base, uint16_t Selector, uint8_t Flags);
#endif
