#ifndef MAINFUNCPRIVATE
#define MAINFUNCPRIVATE
#include <Arduino.h>
#include <WebSocketsServer.h>

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);


#endif