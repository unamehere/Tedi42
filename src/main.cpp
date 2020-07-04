#include <Arduino.h>
#include "mainFunc/mainFunc.h"
#include "constants.h"
#include <WiFiClientSecure.h>

void setup() {
  m_init();
  // put your setup code here, to run once:
}

void loop()
 {
  wifiServerLoop();
  String Command = "";
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    // add it to the inputString:
     Command += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      setCommFlag(true);
    }
  }

  WiFiServer* socket = getServer();
  WiFiClient client = socket->available();
  if(client)
  {
    Command = "";
    while(client.connected())
    {
      if(client.available())
      {
        setWebSocketConnectedFlag(true);
        char c = client.read();            
        Command += c;
        if(c == '\n' && Command.length() > 1)
        {
          String resp = handleNewCommand(Command);
          client.print(resp.c_str());
          Command = "";
          delay(10);
        }
      }
    }
    setWebSocketConnectedFlag(false);
    client.stop();
  }

  if(getCommFlag())
  {
    setCommFlag(false);
    Serial.print(handleNewCommand(Command));
  }
  // put your main code here, to run repeatedly:
}