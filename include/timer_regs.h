#ifndef TIMER_REGS_H
#define TIMER_REGS_H

#include "bcm2837.h"

#define TIME_BASE (MMIO_BASE + 0x3000)

#define TIMER_CLO (TIME_BASE + 0x04)
#define TIMER_CHI (TIME_BASE + 0x08)

#endif 

