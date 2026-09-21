#include "idt.h"
#include "types.h"
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

void SetIDTDescriptor(uint8_t Number, uint32_t Base, uint8_t Flags){ //Configurates Each Entry(256 Entries) From The IDT, Each One Has 8 Bytes
    IDT[Number].ISRLow = (Base & 0xFFFF);
    IDT[Number].Selector = 0x08;
    IDT[Number].Reserved = 0;
    IDT[Number].Attributes = Flags;
    IDT[Number].ISRHigh = Base >> 16; //(Base >> 16) & 0xFFFF Also Works The Same Way
}

void InitIDT(){
    IDTR.Base = (uint32_t)&IDT;
    IDTR.Limit = (uint16_t)sizeof(IDTGate) * 256 - 1;
    for(uint32_t i = 0; i < 256; i++){ //Removes All Garbage From The IDT Entries
        SetIDTDescriptor(i, 0, 0, 0);
    }
    __asm__ __volatile__("lidt (%0)" : : "r"(&IDTR)); //Loads The IDT
}

inline void outb(uint16_t Port, uint8_t Data){
    __asm __volatile__("outb %0, %1" : : "a"(Data), "Nd"(Port)); //Sends Data To The Input/Output Port 0x60
}

inline void inb(uint16_t Port){
    uint8_t Return
    __asm__ __volatile__("inb %1, %0" : "=a"(Return) : "Nd"(Port)); //Sends The Value From The Input/Output Port(0x60) Inside The Return
    return Return;
}


