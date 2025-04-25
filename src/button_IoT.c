// standard libraries
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// useful libraries
#include "button.h"
#include "sensor_pir.h"

// struct to storing the state of devices
typedef struct {
    int button_a, sensor_pir;
} StateDevices;

// instance of StateDevices
StateDevices *state_devices = NULL;

// struct to timer
struct repeating_timer read_devices_timer;

// callback to read devices
bool read_devices_callback() {

    // update state of button
    state_devices->button_a = button_read();
    printf("state button: %s\n", state_devices->button_a == 0 ? "on" : "off");

    // update state of sensor pir
    state_devices->sensor_pir = sensor_pir_read();
    printf("state sensor pir: %s\n", state_devices->sensor_pir == 0? "on" : "off");

    return true;    // keep reading
}

// function to configure timer for read devices
void read_button_and_sensor_in_ms(int32_t ms) {
    // setting timer interrupt to read button and sensor
    add_repeating_timer_ms(ms, read_devices_callback, NULL, &read_devices_timer);
}

// function to start all devices
void setup() {

    // starting serial communication
    stdio_init_all();

    // starting button A
    button_init();

    // starting sensor pir
    sensor_pir_init();

    // creation of state devices with initial values
    state_devices = (StateDevices*) malloc(sizeof(StateDevices));
    state_devices->button_a = 0;
    state_devices->sensor_pir = 0;
}

int main()
{

    // initializing devices
    setup();
    // start reading devices every 1 second
    read_button_and_sensor_in_ms(1000);

    while (true) {
        tight_loop_contents();
    }
}
