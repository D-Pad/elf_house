#ifndef MMIO_H
#define MMIO_H

#include <stdint.h>

// Helper function for writing data to a specific register
static inline void mmio_write(uintptr_t reg, uint32_t value)
{
    *(volatile uint32_t *)reg = value;
}

// Helper function for reading data from a specific register
static inline uint32_t mmio_read(uintptr_t reg)
{
    return *(volatile uint32_t *)reg;
}

// Helper function for changing a pin while preserving the configuration
// of the other 9 pins
static inline void mmio_write_mask(uintptr_t reg,
                                   uint32_t clear_mask,
                                   uint32_t set_mask)
{
    uint32_t value = mmio_read(reg);

    value &= ~clear_mask;
    value |= set_mask;

    mmio_write(reg, value);
}



#endif
