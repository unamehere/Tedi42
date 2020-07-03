#include <Arduino.h>
#include "mainFunc/mainFunc.h"
#include "constants.h"

void setup() {
  m_init();
  // put your setup code here, to run once:
}

void loop()
 {
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
        if(c == '\n')
        {
          String resp = handleNewCommand(Command);
          client.write(resp.c_str());
          Command = "";
        }
      }
    }
    setWebSocketConnectedFlag(false);
    client.stop();
  }

  if(getCommFlag())
  {
    setCommFlag(false);
    handleNewCommand(Command);
  }
  // put your main code here, to run repeatedly:
}