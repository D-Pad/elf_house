#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_put_hex(uint32_t value);
void uart_dump_reg(const char *name, uintptr_t address);

#endif

