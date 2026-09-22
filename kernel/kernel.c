#include "../headers/idt.h"
#include "../headers/types.h"
#include "../headers/vga.h"

void kmain(){
    ClearScreen();
    KPrintln("Kernel Loaded!", VGA_GREEN);
    KPrintln("Welcome To <:(NICO_OS):>", VGA_PURPLE);
}
