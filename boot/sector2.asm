BITS 16
org 0x7E00
    xor ax, ax
    mov ds, ax
    mov [BootDrive], dl
    mov ax, 0x1000
    mov es, ax
    mov bx, 0x0000

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

    mov ah, 0x02
    mov al, 0x0F ;15
    mov ch, 0x00
    mov cl, 0x03
    mov dh, 0x00
    mov dl, [BootDrive]
    int 0x13

    jmp InitializeProtectedMode
    GdtStart:
        NullDescriptor:
            dq 0x00
        CodeDescriptor:
            dw 0xFFFF ;Limit - 2 Bytes
            dw 0x00 ;Base - 2 Bytes
            db 0x00 ;Base - 1 Byte
            db 10011010b ;Type - 1 Byte
            db 11001111b ;Flags - 1 Byte
            db 0x00 ;Base - 1 Byte
        DataDescriptor:
            dw 0xFFFF ;Limit - 2 Bytes
            dw 0x00 ;Base - 2 Bytes
            db 0x00 ;Base - 1 Byte
            db 10010010b ;Type - 1 Byte
            db 11001111b ;Flags - 1 Byte
            db 0x00 ;Base - 1 Byte
    GdtEnd:

    GdtDescriptor:
        dw GdtEnd - GdtStart - 1 ;Offset máximo da GDT
        dd GdtStart
    InitializeProtectedMode:
        cli
        lgdt [GdtDescriptor]
        mov eax, cr0 ;o CR0(Control Register 0) que faz a ignição do modo 32 Bits, quando ele fica 1 significa que vira 32 Bits
        or eax, 1 ;compara o EAX que tem o CR0 dentro com 1, se tiver qualquer 1 em binário neles vira 1, o resultado que é guardado no primeiro bit de eax que é a chave para o protected mode
        mov cr0, eax ;manda as configurações seguras e corretas pra cr0
        jmp 0x08:ProtectedModeStart ;vai direto pro byte 8 do nosso descriptor que é o CodeDescriptor, 0x08 é 8, então é 8 Bytes



    BITS 32
    ProtectedModeStart:
        mov ax, 0x10 ;16 em hexadecimal, que fica 16 Bytes, pula pro Byte 16 da tabela da GDT que é o nosso Data Descriptor
        mov ds, ax ;configura todos os segmentos que tem para o Protected Mode de 32 Bits para deixá-los seguros
        mov es, ax ;configura um extra segment especial
        mov fs, ax ;configura outro extra segment especial
        mov gs, ax ;configura outro extra segment especial
        mov ss, ax ;configura o stack segment

        mov esp, 0x90000 ;escolhemos um endereço de memória que seguro pois fica longe do 0x7C00 e 0x7E00
        jmp 0x10000
BootDrive db 0
times 512 - ($ - $$) db 0
