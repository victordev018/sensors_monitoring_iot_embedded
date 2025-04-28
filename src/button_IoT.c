// standard libraries
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

// useful libraries
#include "button.h"
#include "hcsr04.h"
#include "display.h"
#include "wifi.h"
#include "server.h"

// struct to storing the state of devices
typedef struct {
    int button_a;
    float sensor_distance;
} StateDevices;

// instance of StateDevices
StateDevices *state_devices = NULL;

// function to show the status of devices on the display
void update_device_status_on_display(int button_status, float distance) {
    // clear display
    display_clear();

    // recording header on display
    display_write("Device Status", 22, 0, 1);

    // recording the button status on the display
    char button_msg[12];
    snprintf(button_msg, sizeof(button_msg), "Button: %s", button_status == 0 ? "ON" : "OFF");
    display_write(button_msg, 0, 20, 1);

    // recording distance on the display
    char distance_msg[25];
    snprintf(distance_msg, sizeof(distance_msg), "Distance: %.2f cm", distance);
    display_write(distance_msg, 0, 35, 1);

    // draw content on display
    display_show();
}

// function to start all devices
void setup() {

    // starting serial communication
    stdio_init_all();

    // starting display
    display_init();

    // starting button A
    button_init();

    // starting sensor of distance
    hcsr04_init();

    // creation of state devices with initial values
    state_devices = (StateDevices*) malloc(sizeof(StateDevices));
    state_devices->button_a = 0;
    state_devices->sensor_distance = 0.0;
}

int main()
{

    // initializing devices
    setup();
    sleep_ms(1500);
    // trying to connect to wifi a network
    wifi_connect();
    server_create_tcp_connection();

    while (true) {

        // update state of button
        state_devices->button_a = button_read();

        // update value of sensor distance
        state_devices->sensor_distance = hcsr04_get_distance();

        // show state on display
        update_device_status_on_display(state_devices->button_a, state_devices->sensor_distance);

        // send data to server
        server_send_data_to_server(state_devices->button_a, state_devices->sensor_distance);

        // wait one second
        sleep_ms(1000);
    }
}
