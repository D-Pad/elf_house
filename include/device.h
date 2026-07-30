#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    char* name;
    uint8_t id;

    uint32_t trigger_pin;
    uint32_t finish_pin;

} device_t;


void device_init(device_t *device);

void device_trigger(device_t *device);

bool device_is_done(device_t *device);

void device_wait(device_t *device);

#endif
