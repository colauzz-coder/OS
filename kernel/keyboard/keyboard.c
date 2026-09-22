#include "../headers/io.h"
#include "../headers/types.h"
void KeyboardHandler(void){
    uint8_t Scancode = inb(0x60); //Number 0x60 Is The Input/Output Port That We Use For The Keyboard, Necessary For Receiving New Keys
    outb(0x20, 0x20); //Sends The EOI(End Of Interrupt) Signal To The Master PIC, It Sends The Byte 0x20 To The Port 0x20(PIC1_COMMAND)
}
