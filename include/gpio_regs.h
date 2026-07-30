
#ifndef GPIO_REGS_H
#define GPIO_REGS_H

// There's no need to define the other GPFSEL registers because they're all 
// contiguous in memory. Every register is 4 bytes apart and can be calculated
// by shifting GPFSEL0 by 4 * registerNum places.
#define GPFSEL0 (GPIO_BASE + 0x00)

#define GPSET0  (GPIO_BASE + 0x1C)
#define GPSET1  (GPIO_BASE + 0x20)

#define GPCLR0  (GPIO_BASE + 0x28)
#define GPCLR1  (GPIO_BASE + 0x2C)

#define GPLEV0  (GPIO_BASE + 0x34)
#define GPLEV1  (GPIO_BASE + 0x38)

#endif 

