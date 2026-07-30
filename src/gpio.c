#include "mmio.h"
#include "gpio.h"
#include "gpio_regs.h"


void gpio_set_function(uint32_t pin,
                       gpio_function_t function)
{
    uint32_t register_index = pin / 10;
    uint32_t shift = (pin % 10) * 3;

    uintptr_t reg = GPFSEL0 + (register_index * 4);

    uint32_t mask = 0b111 << shift;
    uint32_t value = ((uint32_t)function) << shift;

    mmio_write_mask(reg, mask, value);
}


void gpio_set_high(uint32_t pin)
{
    uintptr_t reg = (pin < 32) ? GPSET0 : GPSET1;
    mmio_write(reg, 1u << (pin % 32));
}


void gpio_set_low(uint32_t pin)
{
    uintptr_t reg = (pin < 32) ? GPCLR0 : GPCLR1;
    mmio_write(reg, 1u << (pin % 32));
}


bool gpio_read(uint32_t pin)
{
    uintptr_t reg = (pin < 32) ? GPLEV0 : GPLEV1;
    return (mmio_read(reg) & (1u << (pin % 32))) != 0;
}

