#include "uart.h"



void kernel_main(void) {
   
    uart_init();
    uart_puts("Testing 123.\n");

    while (1) {

    }
}

