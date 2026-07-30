#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "mmio.h"
#include "timer_regs.h"

uint64_t timer_get_ticks(void);
void timer_delay_us(uint32_t us);
void timer_delay_ms(uint32_t ms);

#endif 

