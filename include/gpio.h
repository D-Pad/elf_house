#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    GPIO_INPUT  = 0b000,
    GPIO_OUTPUT = 0b001,
    GPIO_ALT5   = 0b010,
    GPIO_ALT4   = 0b011,
    GPIO_ALT0   = 0b100,
    GPIO_ALT1   = 0b101,
    GPIO_ALT2   = 0b110,
    GPIO_ALT3   = 0b111

} gpio_function_t;

void gpio_set_function(uint32_t pin, gpio_function_t function);

void gpio_set_high(uint32_t pin);

void gpio_set_low(uint32_t pin);

bool gpio_read(uint32_t pin);

#endif
