#include "timer.h"

uint64_t timer_get_ticks(void) {

    uint32_t hi;
    uint32_t lo;

    do {
        hi = mmio_read(TIMER_CHI);
        lo = mmio_read(TIMER_CLO);
    } while (hi != mmio_read(TIMER_CHI));

    return ((uint64_t)hi << 32) | lo;
}


void timer_delay_us(uint32_t us) {
    uint64_t start = timer_get_ticks();
    while ((timer_get_ticks() - start) < us) {}
};


void timer_delay_ms(uint32_t ms) {
    timer_delay_us(ms * 1000);
};

