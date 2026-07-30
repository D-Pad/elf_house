#include "uart.h"



void kernel_main(void) {
   
    uart_init();
    uart_puts("Hello from bare metal RPI3!\n");

    while (1) {

    }
}

