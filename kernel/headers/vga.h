#ifndef VGA_H
#define VGA_H
#define VGA_ADDRESS 0xB8000
#define MaxColumns 80
#define MaxRows 25

#define VGA_WHITE 0x0F
#define VGA_BLACK 0x0
#define VGA_GREEN 0x2
#define VGA_PURPLE 0x05
void KPrint(char* Text, volatile uint8_t Color);
void KPrintln(char* Text, volatile uint8_t Color);
void ClearScreen();
#endif
