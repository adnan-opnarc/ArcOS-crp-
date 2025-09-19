// =============================
// Multiboot header (GRUB loads this)
// =============================
__attribute__((section(".multiboot")))
const unsigned long multiboot_header[] = {
    0x1BADB002,  // magic
    0x00,        // flags
    -(0x1BADB002 + 0x00)  // checksum
};

// kernel.c - CryptonOS kernel that loads shell
#include <stdint.h>
#include <stddef.h>
#include "kernel.h"
#include "crp/shell.h"  // Include shell header
// =============================
// VGA text mode
// =============================
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;
static uint8_t vga_row = 0;
static uint8_t vga_col = 0;
static uint8_t vga_color = 0x0F;

static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

void vga_putc(char c) {
    if (c == '\n') { vga_col = 0; vga_row++; }
    else {
        VGA_BUFFER[vga_row * VGA_WIDTH + vga_col] = vga_entry(c, vga_color);
        vga_col++;
        if (vga_col >= VGA_WIDTH) { vga_col = 0; vga_row++; }
    }
}

void vga_print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) vga_putc(str[i]);
}

// =============================
// Include shell header
// =============================
#include "crp/shell.h"  // Make sure shell.c/h exist
void shell_main(void);  // Prototype for shell entry
// =============================
// Kernel entry
// =============================
void kernel_main(uint32_t magic, uint32_t addr) {
    (void)magic;
    (void)addr;

    vga_print("Booting CryptonOS...\n\n");

    // Load and run the shell
    #ifdef shell_main
    vga_print("[OK] Loading shell...\n\n");
    shell_main();  // This should run your shell in an infinite loop
    #else
    vga_print("[ERR] No shell module found!\n");
    for (;;);  // Halt if no shell
    #endif
}
