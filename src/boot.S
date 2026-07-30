
.section ".text.boot"


.global _start


.extern __stack_top
.extern __bss_start
.extern __bss_end


_start:

    // Initialize the stack pointer 
    ldr x0, =__stack_top
    mov sp, x0

    // Clear the .bss section
    ldr x0, =__bss_start
    ldr x1, =__bss_end


clear_bss:
    
    cmp x0, x1
    b.ge bss_done

    str xzr, [x0], #8

    b clear_bss


bss_done:

    // Jump into our main program located in main.c 
    bl kernel_main


// If kernel main ever returns, stop here forever.
hang:
    
    wfe  // Wait for event (wfe). Low-power idle
    b hang

