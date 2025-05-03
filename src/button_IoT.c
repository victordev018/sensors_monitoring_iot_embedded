// standard libraries
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "string.h"

// useful libraries
#include "button.h"
#include "hcsr04.h"
#include "joystick.h"
#include "display.h"
#include "wifi.h"
#include "server.h"

// struct to storing the state of devices
typedef struct {
    int button_a, x_axis, y_axis;
    char direction[20];
    float sensor_distance;
} StateDevices;

// instance of StateDevices
StateDevices *state_devices = NULL;

// function to init wifi connection and show feedback on display
int init_wifi_connection() {

    // displaying the name of the wifi network
    display_clear();
    display_write("Conectando em:", 23, 20, 1);
    display_write(WIFI_SSID, 23, 32, 1);
    display_show();

    // trying to connect to the network and retrieving connection status
    int state_connection = wifi_connect();

    // show state of connection on display
    display_clear();
    if (state_connection == 0) {
        display_write("CONEXAO ESTABECIDA", 10, 30, 1);
        printf("IP do dispositivo: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
    } else {
        display_write("FALHA NA CONEXAO", 10, 30, 1);
    }
    display_show();
    sleep_ms(3000);

    return state_connection;
}

// function to show the status of devices on the display
void update_device_status_on_display(int button_status, float distance, int axis_x, int axis_y, char *direction) {
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
    display_write(distance_msg, 0, 36, 1);

    // recording data from jostick and simulate compass rose
    char joystick_data_msg[40];
    snprintf(joystick_data_msg, sizeof(joystick_data_msg), "X:%d Y:%d - %s", axis_x, axis_y, direction);
    display_write(joystick_data_msg, 0, 54, 1);

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

    // starting joystick
    joystick_init();

    // creation of state devices with initial values
    state_devices = (StateDevices*) malloc(sizeof(StateDevices));
    state_devices->button_a = 1;
    state_devices->sensor_distance = 0.0;
    state_devices->x_axis = 0;
    state_devices->y_axis = 0;
}

int main()
{

    // initializing devices
    setup();
    sleep_ms(1500);
    
    // trying to connect to wifi a network
    if (init_wifi_connection() == 0){
        // creates tcp connection to server
        server_create_tcp_connection();
    }

    while (true) {

        // update state of button
        state_devices->button_a = button_read();

        // update value of sensor distance
        state_devices->sensor_distance = hcsr04_get_distance();

        // update axis_x, axis_y and direction
        state_devices->x_axis = joystick_read_x();
        state_devices->y_axis = joystick_read_y();
        strcpy(state_devices->direction, joystick_read_direction());

        // show datas on display
        update_device_status_on_display(
        state_devices->button_a, 
        state_devices->sensor_distance,
        state_devices->x_axis,
        state_devices->y_axis,
        state_devices->direction
        );

        // send data to server
        server_send_data_to_server(
        state_devices->button_a,
        state_devices->sensor_distance,
        state_devices->x_axis,
        state_devices->y_axis,
        state_devices->direction
        );

        // wait one second
        sleep_ms(1500);
    }
}
