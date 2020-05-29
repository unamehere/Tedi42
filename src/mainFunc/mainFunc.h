

#ifndef MAINFUNC
#define MAINFUNC
#include "../MLX90621/MLX90621.h"
#include <Arduino.h>


// Main Functions
bool m_init();
bool initTemp();
bool initServo();
void serialSendTemps();
void m_measure();

#endif