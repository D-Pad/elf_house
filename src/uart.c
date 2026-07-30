#include "mmio.h"
#include "uart.h"
#include "uart_regs.h"
#include "gpio.h"


// UART Initialization method. It will be very important to change this 
// when moving from a virtual machine to actual hardware. For now, it's 
// left blank because QEMU starts with UART in a usable state
void uart_init(void)
{

    gpio_set_function(14, GPIO_ALT0);
    gpio_set_function(15, GPIO_ALT0);

    /* Disable UART */
    mmio_write(UART_CR, 0);

    /* Clear interrupts */
    mmio_write(UART_ICR, 0x7FF);

    /* Set baud rate */
    mmio_write(UART_IBRD, 26);
    mmio_write(UART_FBRD, 3);

    /* 8-bit, FIFO enabled */
    mmio_write(UART_LCRH,
               (3 << 5) | (1 << 4));

    /* Enable TX, RX, UART */
    mmio_write(UART_CR,
               (1 << 0) |
               (1 << 8) |
               (1 << 9));
}

// Function to 'put character' into a specific memory address
void uart_putc(char c)
{
    if (c == '\n') {
        uart_putc('\r');
    }

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


void uart_put_hex(uint32_t value) {

    static const char hex[] = "0123456789ABCDEF";
    uart_puts("0x");

    // Each hexidecimal digit is 4 bits. Extract one "nibble" at a time.
    for (int i = 28; i >= 0; i -= 4) {
        uint32_t nibble = (value >> i) & 0xF;
        uart_putc(hex[nibble]);
    }
    
};


void uart_dump_reg(const char *name, uintptr_t address) {
    
    uart_puts(name);
    uart_puts(" = ");

    uart_put_hex(mmio_read(address));

    uart_puts("\n");
}

