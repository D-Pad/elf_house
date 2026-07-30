#include "device.h"
#include "gpio.h"
#include "timer.h"


// Pass a device reference to the functions below to trigger their events 

void device_init(device_t *device)
{
    gpio_set_function(
        device->trigger_pin,
        GPIO_OUTPUT
    );

    gpio_set_function(
        device->done_pin,
        GPIO_INPUT
    );


    // Make sure trigger starts LOW

    gpio_set_low(device->trigger_pin);
}


void device_trigger(device_t *device)
{
    // Create a rising edge
    gpio_set_high(device->trigger_pin);

    timer_delay_ms(10);

    gpio_set_low(device->trigger_pin);
}


bool device_is_done(device_t *device)
{
    return gpio_read(device->done_pin);
}


void device_wait(device_t *device)
{
    while (!device_is_done(device))
    {
        // Wait
    }
}


