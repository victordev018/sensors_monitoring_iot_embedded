#ifndef DISPLAY_H
#define DISPLAY_H

// inclusions libraries
#include "hardware/i2c.h"

// pinout definitions and configurations
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_ADDRESS 0x3C
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_CHANNEL i2c1

// functions definitions
void display_init();
void display_write(const char *msg, uint x, uint y, uint size);
void display_show();
void display_clear();

#endif