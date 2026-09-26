#include "../drivers/keyboard/headers/idt.h"
#include <stdint.h>
#include "../drivers/vga/headers/vga.h"
#include "../drivers/keyboard/keyboard.c"
extern void* KeyboardHandlerStub(void);
void kmain(){
    ClearScreen();
    KPrintln("Kernel Loaded!", VGA_GREEN);
    KPrintln("Welcome To <:(NICO_OS):>", VGA_PURPLE);

    //SetIDTDescriptor(33, KeyboardHandlerStub, 0x8E);
    InitIDT();
    KeyboardHandler();
    __asm__ __volatile__("sti");

}
