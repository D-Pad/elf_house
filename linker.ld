
ENTRY(_start)

SECTIONS
{
    /* The Raspberry Pi firmware loads kernel8.img here */
    . = 0x80000;

    .text :
    {
        KEEP(*(.text.boot))
        *(.text*)
    }

    .rodata :
    {
        *(.rodata*)
    }

    .data :
    {
        *(.data*)
    }

    .bss :
    {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }

    _end = .;

    
    /* Setup the stack */
    . = ALIGN(16);

    __stack_bottom = .;
    . += 0x4000;  /* 16 KB stack */
    __stack_top = .;

}
