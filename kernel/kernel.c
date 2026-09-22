#include "../headers/idt.h"
#include "../headers/types.h"
#include "../headers/vga.h"
extern void KeyboardHandlerStub(void);
void kmain(){
    ClearScreen();
    KPrintln("Kernel Loaded!", VGA_GREEN);
    KPrintln("Welcome To <:(NICO_OS):>", VGA_PURPLE);
    
    SetIDTDescriptor(33, (uint32_t)KeyboardHandlerStub, 0x08, 0x8E);
    __asm__ __volatile__("sti");
    //Continue Later With The Scancode
}
