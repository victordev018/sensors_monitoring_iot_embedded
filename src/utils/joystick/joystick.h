#ifndef JOYSTICK_H
#define JOYSTICK_H

// pinout definitions
#define JOYSTICK_X 27
#define JOYSTICK_Y 26

// definition of functions
void joystick_init();
int joystick_read_x();
int joystick_read_y();
char *joystick_read_direction();

#endif