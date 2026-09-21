#include "idt.h"
#include "types.h"
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
        uint32_t NextRow = 0xB8000 + 160;
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

typedef struct{
    uint16_t ISRLow; //The ISR(Handler) Address Is 32 Bit, But It Is Separated Into 2 16 Bit Parts, This Is The Lower Part
    uint16_t Selector; //The GDT Segment Selector That The CPU Will Load Into Before Calling The ISR(Handler), It Uses The Code Segment(CS)
    uint8_t Reserved; //We Need This But It Is Set To 0
    uint8_t Attributes; //Informs The Gate Properties, Such As P, DPL, Type
    uint16_t ISRHigh; //The Higher Part Of The ISR(Handler) Address(32 Bit)
} __attribute__((packed)) IDTGate;
static IDTGate IDT[256]; //IDT Has 256 Inputs/Entries

typedef struct{
    uint16_t Limit; //Size Of The IDT - 1
    uint32_t Base; //IDT Address
} __attribute__((packed)) IDTRStruct;
static IDTRStruct IDTR;

void SetIDTDescriptor(uint8_t Number, uint32_t Base, uint16_t Selector, uint8_t Flags){
    IDT[Number].ISRLow = (Base & 0xFFFF);
    IDT[Number].Selector = 0x08;
    IDT[Number].Reserved = 0;
    IDT[Number].Attributes = Flags;
    IDT[Number].ISRHigh = Base >> 16;
}

void InitIDT(){
    IDTR.Base = (uint32_t)&IDT;
    IDTR.Limit = (uint16_t)sizeof(IDTGate) * 256 - 1;
    for(uint32_t i = 0; i < 256; i++){
        SetIDTDescriptor(i, 0, 0, 0);
    }
    __asm__ __volatile__("lidt %0" : : "m"(IDTR));
    __asm__ __volatile__("sti");
}

void kmain(){
    ClearScreen();
    KPrintln("Kernel Loaded!", VGA_GREEN);
    KPrintln("Welcome To <:(NICO_OS):>", VGA_PURPLE);
}
