#include "io.h"
#include "pic.h"
#define PIC1_COMMAND 0x20 //Master PIC
#define PIC1_DATA 0x21 //Master PIC
#define PIC2_COMMAND 0xA0 //Slave PIC
#define PIC2_DATA 0xA1 //Slave PIC

void RemapPIC(void){
    uint8_t Mask1 = inb(PIC1_DATA); //Saves The Data That Was There Before For Both Master And Slave PIC
    uint8_t Mask2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, 0x11); //Resets All The PICs Configurations
    outb(PIC2_COMMAND, 0x11); //Resets It By Changing The Last Bit Config, 0x11 In Binary Is 0001 0001, 0x10(16) Is The INIT Signal, It Makes The PIC Reset His State Machine Because We Are Going To Send A Sequence Of 3 Configuration Datas, 0x01(1) Is Another Signal, It Says To The PIC That We Are Going To Send A Fourth Configuration Data In The End

    outb(PIC1_DATA, 0x20); //32, Configurates The First Line Of The PIC Will Start At 32(IRQ 0 -> Timer)
    outb(PIC2_DATA, 0x28); //40, The Master PIC Starts From 32 And Goes To 40, The Master PIC Has 8 IRQs, The Slave PIC Starts At 40, We Set Those Values Because The PIC Points To Places That The CPU Uses For Errors And Exceptions, The First Number That Leaves That Place Is The 32(0 -> 31 / 32 -> 256)

    outb(PIC1_DATA, 0x04); //0x04 In Binary Is 0000 0100, The Bit 2 Is Turned On(1), That Means That Theres A Slave PIC In The IRQ Line 2(IRQ 2)
    outb(PIC2_DATA, 0x02); //0x02 In Decimal Is 2, Means That The Number Of The Slave PIC Is 2(IRQ 2)

    outb(PIC1_DATA, 0x01); //0x01 In Binary Is 0000 0001, That Turned On Bit(1) Means That The PIC Should Treat All The Configurations Like A x86 Processor
    outb(PIC2_DATA, 0x01); //Same With The Slave PIC

    outb(PIC1_DATA, Mask1); //Gets The Bytes That We Read Before With inb And We Pass The New Ones To It, But Keeping The Ones That We Didn't Use Like Before
    outb(PIC2_DATA, Mask2);
}
