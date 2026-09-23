#include "../../../lib/types.h"
#include "../headers/vga.h"
#define VGA_ADDRESS 0xB8000
#define MaxColumns 80
#define MaxRows 25

#define VGA_WHITE 0x0F
#define VGA_BLACK 0x0
#define VGA_GREEN 0x2
#define VGA_PURPLE 0x05

uint32_t Row = 0;
uint32_t Column = 0;
void KPrint(char* Text, volatile uint8_t Color){
    volatile uint8_t* const VGA = (volatile uint8_t* const)0xB8000;
    uint32_t TextSize = 0;
    while(Text[TextSize] != '\0'){
        TextSize++;
    }
    for(uint32_t i = 0; i < TextSize; i++){
        uint32_t Offset = (Row * 80 + Column) * 2;
        VGA[Offset] = Text[i];
        VGA[Offset + 1] = Color;
        Column++;
    }
}
void KPrintln(char* Text, volatile uint8_t Color){
    volatile uint8_t* const VGA = (volatile uint8_t* const)0xB8000;
    uint32_t TextSize = 0;

    while(Text[TextSize] != '\0'){
        TextSize++;
    }
    Text[TextSize] = '\n';

    for(uint32_t i = 0; i < TextSize; i++){
        if(Text[i] == '\n'){
            Column = 0;
            Row++;
        }
        uint32_t Offset = (Row * 80 + Column) * 2;
        VGA[Offset] = Text[i];
        VGA[Offset + 1] = Color;
        Column++;
    }

    if(Text[TextSize] == '\n'){
        Row++;
        Column = 0;
    }
}

void ClearScreen(){
    Column = 0;
    Row = 0;
    print:
        for(uint32_t i = 0; i < (MaxColumns * 2); i++){
            KPrint(" ", VGA_BLACK);
        }
    Column = 0;
    Row++;
    if(Row != MaxRows){
        goto print;
    }else{
        Row = 0;
        Column = 0;
    }
}
