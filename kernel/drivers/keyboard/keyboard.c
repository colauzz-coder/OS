#include "headers/io.h"
#include <stdint.h>
#include <stdbool.h>
#include "../vga/headers/vga.h"
void KeyboardHandler(void){
    uint8_t Scancode = inb(0x60); //Number 0x60 Is The Input/Output Port That We Use For The Keyboard, Necessary For Receiving New Keys
    static char const KeymapLowercase[] = {
        0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
        '*',   0, ' '
    };
    static char const KeymapUppercase[] = {

        0,   27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
        '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
        0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
        0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0,
        '*',   0, ' '
    };
    bool ShiftPressed = 0;
    if(Scancode & 0x80){ //If The Key Was Released(Break Code)
        uint8_t ReleasedKey = Scancode & ~0x80; //We Need To Separate The Keys That Were Released And The Ones Who Weren't, We Make a NOT Operation In The 0x80 That Becomes 0111 1111(Before: 1000 0000) And Then
        if(ReleasedKey == 0x2A || ReleasedKey == 0x36){ //Checks If The User Has Released The Right Or Left Shift
            ShiftPressed = 0;
        }
    }else{ //If The Key Was Pressed(Make Code)
        if(Scancode == 0x2A || Scancode == 0x36){ //If The Right Or Left Shift Were Pressed
            ShiftPressed = 1;
        }
        char Character = ShiftPressed ? KeymapUppercase[Scancode] : KeymapLowercase[Scancode]; //If Left Or Right Shift Were Pressed, Use The Uppercase Key Map To Print The Characters, Otherwise Use The Lowercase one
        if(Character != 0){ //If The Character Is Different From 0, That Means It Is Valid(There Is No 0 Character, Thats Why In The Key Map We Don't Use The First Element In The Array(0))
            KPrint(&Character, 0x0F); //Prints The Character
        }
    }
    outb(0x20, 0x20); //Sends The EOI(End Of Interrupt) Signal To The Master PIC, It Sends The Byte 0x20 To The Port 0x20(PIC1_COMMAND), If Not Sent, The Keyboard Won't Work
}
