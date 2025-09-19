// shell.c - generated from crp/shell.crp
#include <stdint.h>
#include <stddef.h>
#include "shell.h"
#include "../kernel.h"  // For vga_print

void shell_main(void) {
    vga_print("Welcome to CryptonOS!\n");
    vga_print("hello world\n");
}
