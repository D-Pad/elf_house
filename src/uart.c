#include "uart.h"

#define MMIO_BASE 0x3F000000

#define UART0_BASE (MMIO_BASE + 0x201000)

#define UART0_DR     ((volatile unsigned int *)(UART0_BASE + 0x00))
#define UART0_FR     ((volatile unsigned int *)(UART0_BASE + 0x18))

#define UART_FR_TXFF (1 << 5)

void uart_init(void)
{
    /*
     * For now, leave initialization minimal.
     * QEMU starts the UART in a usable state.
     */
}

void uart_putc(char c)
{
    while (*UART0_FR & UART_FR_TXFF)
    {
        // Wait until transmit FIFO has space
    }

    *UART0_DR = c;
}

void uart_puts(const char *s)
{
    while (*s)
    {
        uart_putc(*s++);
    }
}

