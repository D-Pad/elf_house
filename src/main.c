#include "uart.h"



void kernel_main(void) {
   
    uart_init();
    
    uart_puts("\n");
    uart_puts("========================\n");
    uart_puts(" Raspberry Pi 3 Booting\n");
    uart_puts("========================\n");

    uart_puts("UART initialized\n");

    while (1) {

    }
}

