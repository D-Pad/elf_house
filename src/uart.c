#include "uart.h"
#include "mmio.h"
#include "bcm2837.h"
#include "uart_regs.h"


// UART Initialization method. It will be very important to change this 
// when moving from a virtual machine to actual hardware. For now, it's 
// left blank because QEMU starts with UART in a usable state
void uart_init(void)
{

}

// Function to 'put character' into a specific memory address
void uart_putc(char c)
{
    while (mmio_read(UART0_FR) & UART_FR_TXFF)
    {
        // Wait until transmit FIFO has space
    }

    mmio_write(UART0_DR, c);
}

// Function to write an array of characters (c string) into a multiple
// memory addresses. Basically a print function
void uart_puts(const char *s)
{
    while (*s)
    {
        uart_putc(*s++);
    }
}

