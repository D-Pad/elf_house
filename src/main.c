#include "uart.h"
#include "timer.h"
#include "device.h"


// Define pico devices here
device_t toy_making_elves = {
    .name = "toy makers",
    .id = 1,
    .trigger_pin = 17,
    .finish_pin = 18
};


device_t toy_packaging_elves = {
    .name = "toy packagers",
    .id = 2,
    .trigger_pin = 19,
    .finish_pin = 20
};


// Main function
void kernel_main(void) {

    // Initialization
    uart_init();
    
    uart_puts("\n");
    uart_puts("========================\n");
    uart_puts(" Raspberry Pi 3 Booting\n");
    uart_puts("========================\n");

    uart_puts("UART initialized\n");

    // Group devices
    device_t* picos[] = {
        &toy_making_elves,
        &toy_packaging_elves
    };

    const int numDevices = sizeof(picos) / sizeof(picos[0]);

    // Initialize devices
    for (int i = 0; i < numDevices; i++) {
        device_t* pico = picos[i]; 
        device_init(pico);
        uart_puts("Initialized ");
        uart_puts(pico->name);
        uart_puts("\n");
    } 

    while (1) {

        timer_delay_ms(100);

        // Check for devices that have finished their loops here, then 
        // retrigger when they're all finished.

    }

    uart_puts("Returning\n");

}

