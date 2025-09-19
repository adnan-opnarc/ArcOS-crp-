#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>

// Function prototypes from kernel.c
void vga_print(const char* str);
void vga_putc(char c);

// Prototype for the entry point of the compiled shell
void shell_main(void);

#endif
