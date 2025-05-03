// inclusion of libraries
#include "joystick.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

// matrix of directions
char* directions[3][3] = {
    {"Sudoeste", "Oeste", "Noroeste"},
    {"Sul", "Centro", "Norte"},
    {"Sudeste", "Leste", "Nordeste"}
};

// implementation of functions
void joystick_init() {
    adc_init();
    adc_gpio_init(JOYSTICK_X);
    adc_gpio_init(JOYSTICK_Y);
}

int joystick_read_x() {
    adc_select_input(1);
    return adc_read() * 100 / 4095;
}

int joystick_read_y() {
    adc_select_input(0);
    return adc_read() * 100 / 4095;
}

char *joystick_read_direction() {

    int deadzone = 10;
    int dx = 0, dy = 0;
    int x = joystick_read_x(), y = joystick_read_y();

    // define direction x [1 or -1]
    if (x > (50 + deadzone)) dx = 1;
    else if (x < (50 - deadzone)) dx = -1;

    // define direction y [1 or -1]
    if (y > (50 + deadzone)) dy = 1;
    else if (y < (50 - deadzone)) dy = -1;

    return directions[dx+1][dy+1];

}