// standard libraries
#include <stdio.h>
#include "pico/stdlib.h"

// useful libraries
#include "button.h"

// callback to handler gpio interrupts
void global_handler_irq(uint pin, uint32_t event) {
    if (pin == BUTTON_A_PIN) {
        printf("button A was pressed");
    }
}

// function to start all devices
void setup() {

    // starting serial communication
    stdio_init_all();

    // starting button A
    button_init();

    // setting interrupt on button A
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, global_handler_irq);
}

int main()
{

    // initializing devices
    setup();

    while (true) {
        sleep_ms(1000);
    }
}
