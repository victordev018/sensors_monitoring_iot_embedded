// inclusion libraries
#include "pico/stdlib.h"
#include "button.h"

// implementation of functions
void button_init() {
    // initializing button A
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);                 // set default HIGH
}