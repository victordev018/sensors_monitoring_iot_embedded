// inclusion of libraries
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hcsr04.h"

// implementation functions
void hcsr04_init() {
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}

float hcsr04_get_distance() {
    uint64_t start_time, end_time;
    float total_time, distance;

    // ensure TRIG in LOW
    gpio_put(TRIG_PIN, 0);
    sleep_us(2);

    // send pulse of 10us
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    // wait for ECHO to rise
    while(gpio_get(ECHO_PIN) == 0);
    start_time = time_us_64();          // start time mark

    // wait for ECHO to go down
    while(gpio_get(ECHO_PIN) == 1);
    end_time = time_us_64();            // end time mark

    // calculates time and distance
    total_time = (float) (end_time - start_time);
    distance = (total_time * 0.0343) / 2.0;

    return distance;
}