// inclusions libraries
#include <stdio.h>
#include "pico/stdlib.h"
#include "display.h"
#include "ssd1306.h"

// instance of dislay
ssd1306_t display;

void i2c_start() {
    // Initialize I2C no channel 1
    i2c_init(I2C_CHANNEL, 400 * 1000); // 400 kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

void display_init() {
    i2c_start();
    if (!ssd1306_init(&display, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_ADDRESS, I2C_CHANNEL)) {
        printf("Failed to init display\n");
        return;
    }
    printf("Display started successfully");
}

void display_write(const char *msg, uint x, uint y, uint size) {
    ssd1306_draw_string(&display, x, y, size, msg);
}

void display_show() {
    ssd1306_show(&display);
}

void display_clear() {
    ssd1306_clear(&display);
}
