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
  ArduinoOTA.handle();
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
    while(client.connected())
    {
      if(client.available())
      {
        setWebSocketConnectedFlag(true);
        unsigned char c[6];
        unsigned char* cmd = c;
        int len =  client.read(cmd, 6);          
        if(len > 2) // shortest command: MM\n
        {
          const char* ccmd = (char*)cmd;
          String resp = handleNewCommand(ccmd);
          client.print(resp.c_str());
          client.flush();
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