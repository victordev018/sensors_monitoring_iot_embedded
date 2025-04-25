// inclusion libraries
#include "pico/stdlib.h"
#include "sensor_pir.h"

// implementation of functions
void sensor_pir_init() {
    gpio_init(SENSOR_PIR_PIN);
    gpio_set_dir(SENSOR_PIR_PIN, GPIO_IN);
    gpio_pull_up(SENSOR_PIR_PIN);                 // set default HIGH
}

int sensor_pir_read() {
    return gpio_get(SENSOR_PIR_PIN);
}