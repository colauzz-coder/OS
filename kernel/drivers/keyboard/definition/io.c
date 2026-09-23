#include "../headers/io.h"
void outb(uint16_t Port, uint8_t Data){
    __asm__ __volatile__("outb %0, %1" : : "a"(Data), "Nd"(Port)); //Sends Data To The Input/Output Port 0x60
}

uint8_t inb(uint16_t Port){
    uint8_t Return;
    __asm__ __volatile__("inb %1, %0" : "=a"(Return) : "Nd"(Port)); //Sends The Value From The Input/Output Port(0x60) Inside The Return
    return Return;
}
