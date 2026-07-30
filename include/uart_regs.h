#ifndef UART0_REGS_H
#define UART0_REGS_H

#include "bcm2837.h"

// UART offsets
#define UART0_DR (UART0_BASE + 0x00)
#define UART0_FR (UART0_BASE + 0x18)

#define UART_IBRD   (UART0_BASE + 0x24)
#define UART_FBRD   (UART0_BASE + 0x28)

#define UART_LCRH   (UART0_BASE + 0x2C)
#define UART_CR     (UART0_BASE + 0x30)

#define UART_IMSC   (UART0_BASE + 0x38)
#define UART_ICR    (UART0_BASE + 0x44)

#define UART_FR_TXFF (1 << 5)

#endif

