#ifndef UART0_REGS_H
#define UART0_REGS_H

// UART data register and flag register
#define UART0_DR (UART0_BASE + 0x00)
#define UART0_FR (UART0_BASE + 0x18)

// Transmit FIFO flag
#define UART_FR_TXFF (1 << 5)

#endif

