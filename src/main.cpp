#include <Arduino.h>
#include "mainFunc/mainFunc.h"
#include "constants.h"

void setup() {
  m_init();
  // put your setup code here, to run once:
}

void loop() {

  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    // add it to the inputString:
    String* Command = getCommand();
     *Command += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      setCommFlag(true);
    }
  }

  if(getCommFlag())
  {
    setCommFlag(false);
    handleNewCommand();
  }

  /*  
  long t1 = millis();
  m_measure();
  serialSendTemps();
  int diff = millis()-t1;
  Serial.print("Duration: ");
  Serial.println(diff);
  delay(1000);
  */
  
  // put your main code here, to run repeatedly:
}