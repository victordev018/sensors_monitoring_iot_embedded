// standard libraries
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// useful libraries
#include "button.h"
#include "sensor_pir.h"
#include "display.h"

// struct to storing the state of devices
typedef struct {
    int button_a, sensor_pir;
} StateDevices;

// instance of StateDevices
StateDevices *state_devices = NULL;

// struct to timer
struct repeating_timer read_devices_timer;

// function to show the status of devices on the display
void update_device_status_on_display(int button_status, int sensor_status) {
    // clear display
    display_clear();

    // recording header on display
    display_write("Device Status", 22, 0, 1);

    // recording the button status on the display
    char button_msg[12];
    snprintf(button_msg, sizeof(button_msg), "Button: %s", button_status == 0 ? "ON" : "OFF");
    display_write(button_msg, 0, 20, 2);

    // recording the sensor pir status on the display
    char sensor_pir_msg[12];
    snprintf(sensor_pir_msg, sizeof(sensor_pir_msg), "Sensor: %s", sensor_status == 0? "ON" : "OFF");
    display_write(sensor_pir_msg, 0, 42, 2);

    // draw content on display
    display_show();
}

// callback to read devices
bool read_devices_callback() {

    // update state of button
    state_devices->button_a = button_read();
    printf("state button: %s\n", state_devices->button_a == 0 ? "on" : "off");

    // update state of sensor pir
    state_devices->sensor_pir = sensor_pir_read();
    printf("state sensor pir: %s\n", state_devices->sensor_pir == 0? "on" : "off");

    // show state on display
    update_device_status_on_display(state_devices->button_a, state_devices->sensor_pir);

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

    // starting display
    display_init();

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
