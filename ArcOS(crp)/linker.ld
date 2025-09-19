ENTRY(kernel_main)

SECTIONS
{
  /* Kernel loads at 1MB */
  . = 1M;

  .text : {
    *(.multiboot_header)   /* Multiboot header for GRUB */
    *(.text*)
  }

  .rodata : {
    *(.rodata*)
  }

  .data : {
    *(.data*)
  }

  .bss : {
    *(.bss*)
    *(COMMON)
  }
}
