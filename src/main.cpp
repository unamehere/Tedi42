#include <Arduino.h>
#include "mainFunc/mainFunc.h"

void setup() {
  m_init();
  // put your setup code here, to run once:
}

void loop() {
  long t1 = millis();
  m_measure();
  serialSendTemps();
  int diff = millis()-t1;
  Serial.print("Duration: ");
  Serial.println(diff);
  delay(1000);
  
  // put your main code here, to run repeatedly:
}