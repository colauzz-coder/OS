BITS 16
org 0x7C00
    xor dh, dh
    xor ax, ax
    mov bp, 0x7E00
    mov sp, bp

    mov ax, 0x0000
    mov es, ax
    mov bx, 0x7E00
    push dx
    push ax
    push bx

    mov [BootDrive], dl

    mov ah, 0x0
    mov al, 0x13
    int 0x10

    mov ah, 0x0C
    mov al, 0x00
    mov bh, 0
    mov cx, 0
    mov dx, 0
    int 0x10

    mov ax, 0x03
    int 0x10

    mov si, BootLoaderLog
    Print:
        mov ah, 0x0E
        mov al, [si]
        int 0x10
        lodsb
        cmp al, 0
        je SecondSector
        jmp Print

    SecondSector:
        mov ah, 0x02
        mov al, 0x01
        mov ch, 0x00
        mov cl, 0x02
        mov dh, 0x00
        mov dl, [BootDrive]
        pop bx
        pop es
        int 0x13
        jc SetPrintSectorError
        jmp SetPrintSectorSuccess
        pop dx
    SetPrintSectorError:
        mov si, SectorErrorLog
    PrintSectorError:
        mov ah, 0x0E
        mov al, [si]
        int 0x10
        lodsb
        cmp al, 0
        je Halt
        jmp PrintSectorError

    SetPrintSectorSuccess:
        mov si, SectorSuccessLog
    PrintSectorSuccess:
        mov ah, 0x0E
        mov al, [si]
        int 0x10
        lodsb
        cmp al, 0
        je LongJump
        jmp PrintSectorSuccess
    Halt:
        jmp $
    LongJump:
        jmp 0x0000:0x7E00

BootLoaderLog db "BootLoader Loaded!", 0

SectorErrorLog db "Error, Couldn't Load Second Sector", 0
SectorSuccessLog db "Second Sector Loaded Successfully", 0

BootDrive db 0

times 510 - ($ - $$) db 0
dw 0xAA55
