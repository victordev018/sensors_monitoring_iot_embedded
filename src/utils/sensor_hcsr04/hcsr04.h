#ifndef HCSR04_H
#define HCSR04_H

// pinout definition
#define TRIG_PIN 17
#define ECHO_PIN 16

// definitions functions
void hcsr04_init();
float hcsr04_get_distance();

#endif