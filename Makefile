#Setup
CC := gcc
ASM := nasm
LD := ld
QEMU := qemu-system-x86_64 #Can Run 32 Bit And Also 64 Bit

#Files
BUILD:= compile
LINKER := linker
KERNEL := kernel
BOOT := boot

#Flags
C_FLAGS := -m32 -ffreestanding -fno-pie -fno-stack-protector -c -Wall -Wextra -O0
LD_FLAGS := -m elf_i386 -T $(LINKER)/linker.ld --oformat binary
ASM_FLAGS := -f elf32
QEMU_FLAGS := -m 128M -drive format=raw,file=$(BUILD)/SO.img

KERNEL_C_SRC := $(shell find $(KERNEL) -type f -name '*.c')
KERNEL_ASM_SRC := $(shell find $(KERNEL) -type f -name '*.asm')
KERNEL_C_OBJ := $(patsubst %.c, $(BUILD)/%.o, $(KERNEL_C_SRC))
KERNEL_ASM_OBJ := $(patsubst %.asm, $(BUILD)/%.asm.o, $(KERNEL_ASM_SRC))
KERNEL_ENTRY := boot/kernel_entry.asm
KERNEL_ENTRY_OBJ := $(BUILD)/kernel_entry.asm.o

$(KERNEL_ENTRY_OBJ): $(KERNEL_ENTRY)
	mkdir -p $(dir $@)
	$(ASM) $(ASM_FLAGS) $< -o $@

$(BUILD)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $< -o $@

$(BUILD)/%.asm.o: %.asm
	mkdir -p $(dir $@)
	$(ASM) $(ASM_FLAGS) $< -o $@

$(BUILD)/bootloader.bin: $(BOOT)/bootloader.asm
	mkdir -p $(dir $@)
	$(ASM) -f bin $< -o $@

$(BUILD)/kernel.bin: $(KERNEL_ENTRY_OBJ) $(KERNEL_C_OBJ) $(KERNEL_ASM_OBJ)
	mkdir -p $(dir $@)
	$(LD) $(LD_FLAGS) $^ -o $@

#$(BUILD)/SO.img: $(BUILD)/bootloader.bin $(BUILD)/kernel.bin
#	mkdir -p $(dir $@)
#	cat $^ > $@
$(BUILD)/sector2.bin: $(BOOT)/sector2.asm
	mkdir -p $(dir $@)
	$(ASM) -f bin $< -o $@

$(BUILD)/SO.img: $(BUILD)/bootloader.bin $(BUILD)/sector2.bin $(BUILD)/kernel.bin
	mkdir -p $(dir $@)
	cat $^ > $@
	truncate -s '>16K' $@
clean:
	rm -rf $(BUILD)

run: $(BUILD)/SO.img
	$(QEMU)$(QEMU_FLAGS)

all: $(BUILD)/SO.img

.PHONY: all clean run
