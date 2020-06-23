

#ifndef MAINFUNC
#define MAINFUNC
#include "../MLX90621/MLX90621.h"
#include <Arduino.h>

// Main Functions
bool m_init();
bool initTemp();
bool initServo();
bool initWebSocket();
void serialSendTemps();
String stringTemps()
String handleNewCommand(String Command);
bool checkDeg(uint8_t ID, uint16_t const &deg);
uint16_t degToInt(uint16_t &deg);


bool getCommFlag();
void setCommFlag(bool flag);

void webSocketLoop();

#endif