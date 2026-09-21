NASM := nasm
CC := gcc
LD := ld
QEMU := qemu-system-x86_64

BUILD := compile
KERNEL := kernel

CFLAGS := -m32 -ffreestanding -fno-pie -fno-stack-protector -c
LDFLAGS := -m elf_i386 -T $(BUILD)/linker.ld --oformat binary

BOOTLOADER_SRC := boot/bootloader.asm
SECTOR2_SRC := boot/sector2.asm
KERNEL_ENTRY_SRC := $(BUILD)/kernelentry.asm
KERNEL_SRC := $(KERNEL)/kernel.c
IDT_SRC := $(KERNEL)/idt.c
LINKER_SCRIPT := $(BUILD)/linker.ld

BOOTLOADER_BIN := $(BUILD)/bootloader.bin
SECTOR2_BIN := $(BUILD)/sector2.bin
KERNEL_ENTRY_OBJ := $(BUILD)/kernelentry.o
KERNEL_OBJ := $(BUILD)/kernel.o
KERNEL_BIN := $(BUILD)/kernel.bin
IDT_OBJ := $(BUILD)/idt.o
OS_IMAGE := $(BUILD)/OS.img

all: $(OS_IMAGE)

$(BOOTLOADER_BIN): $(BOOTLOADER_SRC)
	@mkdir -p $(BUILD)
	$(NASM) -f bin $< -o $@

$(SECTOR2_BIN): $(SECTOR2_SRC)
	@mkdir -p $(BUILD)
	$(NASM) -f bin $< -o $@

$(KERNEL_ENTRY_OBJ): $(KERNEL_ENTRY_SRC)
	@mkdir -p $(BUILD)
	$(NASM) -f elf32 $< -o $@

$(KERNEL_OBJ): $(KERNEL_SRC)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< -o $@

$(IDT_OBJ): $(IDT_SRC)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $< -o $@

$(KERNEL_BIN): $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJ) $(IDT_OBJ) $(LINKER_SCRIPT)
	$(LD) $(LDFLAGS) $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJ) -o $@

$(OS_IMAGE): $(BOOTLOADER_BIN) $(SECTOR2_BIN) $(KERNEL_BIN)
	cat $^ > $@

run: $(OS_IMAGE)
	$(QEMU) -drive format=raw,file=$(OS_IMAGE)

clean:
	rm -f $(BOOTLOADER_BIN) \
	      $(SECTOR2_BIN) \
	      $(KERNEL_ENTRY_OBJ) \
	      $(KERNEL_OBJ) \
		  $(IDT_OBJ) \
	      $(KERNEL_BIN) \
	      $(OS_IMAGE)

.PHONY: all run clean
