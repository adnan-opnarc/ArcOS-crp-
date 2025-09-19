# --- Build Tools ---
CC = gcc
LD = ld
PYTHON = python

# --- Compilation Flags ---
CFLAGS = -m32 -ffreestanding -nostdlib -fno-pic -fno-stack-protector -Wall -Wextra -fno-builtin
LDFLAGS = -T linker.ld -m elf_i386

# --- Paths ---
CRP_DIR = crp
ISO_DIR = iso
BOOT_DIR = $(ISO_DIR)/boot
GRUB_DIR = $(BOOT_DIR)/grub

# --- Find all CRP files ---
CRP_FILES := $(wildcard $(CRP_DIR)/*.crp)
C_GEN := $(CRP_FILES:.crp=.c)
H_GEN := $(CRP_FILES:.crp=.h)

# --- All C sources ---
C_SRCS := $(wildcard *.c) $(C_GEN)
C_OBJS := $(C_SRCS:.c=.o)

KERNEL_ELF = kernel.elf
ISO_NAME = CryptonOS.iso

# --- Phony targets ---
.PHONY: all run iso clean crp

# Default: build and run
all: run

# Compile all CRP files first
crp: $(C_GEN) $(H_GEN)

# --- Generate .c from .crp ---
$(C_GEN): %.c : %.crp
	$(PYTHON) compiler.py $<

# --- Generate .h from .crp ---
$(H_GEN): %.h : %.crp
	$(PYTHON) compiler.py $<

# Compile kernel + all module .c files
$(KERNEL_ELF): crp $(C_OBJS)
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(C_OBJS)

# Compile any C file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run in QEMU
run: $(KERNEL_ELF)
	qemu-system-i386 -kernel $(KERNEL_ELF) -serial stdio

# Build ISO
iso: $(KERNEL_ELF)
	@mkdir -p $(GRUB_DIR)
	cp $(KERNEL_ELF) $(BOOT_DIR)/
	cp $(CRP_DIR)/*.c $(BOOT_DIR)/
	cp boot/grub/grub.cfg $(GRUB_DIR)/
	grub-mkrescue -o $(ISO_DIR)/$(ISO_NAME) $(ISO_DIR)

# Clean
clean:
	rm -f *.o *.elf
	rm -f $(CRP_DIR)/*.c $(CRP_DIR)/*.h
	rm -rf $(ISO_DIR)
