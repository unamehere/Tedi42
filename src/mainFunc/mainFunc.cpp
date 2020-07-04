#include "mainFunc.h"
#include "../pinmap.h"
#include "../constants.h"
#include "../lib/XL320/XL320.h"
#include <SoftwareSerial.h>
#include "mainFunc_private.h"
#include "../lib/WiFiManager/WiFiManager.h"


//globals
MLX90621 tempsensor;
XL320 servo;
WiFiManager wm;
uint16_t uNowPosR;
uint16_t uNowPosT;

WiFiServer server(86);

bool webSocketConnectedFlag = false;

bool bNewCommandFlag;


bool m_init()
{
    bool retVal = true;
    bNewCommandFlag = false;
    Serial.begin(C_COMBAUD);
    retVal = initTemp();
    retVal = initServo();
    retVal = initWebSocket();
    return retVal;
}

bool initTemp()
{
    bool retVal = true;
    tempsensor.setRefreshRate(RATE_16HZ);
    tempsensor.setResolution(RES_17bit);
    tempsensor.setEmissivity(1.0);
    tempsensor.initialize (); // start the thermo cam
    return retVal;
}

bool initServo()
{
    bool retVal = true;
    Serial2.begin(C_SERVOBAUD, SERIAL_8N1, P_SERVO_RX, P_SERVO_TX);
    servo.begin(Serial2);
    servo.setJointSpeed(ID_SERVO_ROT, C_SERVOSPEED);
    delay(50);
    servo.setJointSpeed(ID_SERVO_TILT, C_SERVOSPEED);
    delay(50);
    uNowPosR = servo.getJointPosition(ID_SERVO_ROT);
    delay(50);
    uNowPosT = servo.getJointPosition(ID_SERVO_TILT);
    delay(50);
    return retVal;
}

bool initWebSocket()
{
    bool retVal = true;
    WiFi.mode(WIFI_STA);
    wm.setConfigPortalBlocking(false);
    wm.autoConnect("TED","1234567890");
    IPAddress IP = WiFi.softAPIP();
    server.begin();
    return retVal;
}


void serialSendTemps()
{
    float* temps = tempsensor.getAllTemps();
    Serial.print("TS;");
    for(int y = 0; y < T_ROWCOUNT; y++)
    {
        for(int x = 0; x <T_COLUMNCOUNT; x++)
        {
            Serial.print(temps[y+x*4]);
            if (x<T_COLUMNCOUNT-1) Serial.print(",");
        }
        if(y<T_ROWCOUNT-1) Serial.print(";");
    }
    Serial.println(";TE");
}

String stringTemps()
{
    String resp;
    float* temps = tempsensor.getAllTemps();
    resp += "TS;";
    for(int y = 0; y < T_ROWCOUNT; y++)
    {
        for(int x = 0; x <T_COLUMNCOUNT; x++)
        {
            resp+= temps[y+x*4];
            if (x<15) resp+=",";
        }
        if(y<3) resp+=";";
    }
    resp+=";TE";
    return resp;
}

String handleNewCommand(String Command)
{
    String resp = COM_ANSWER_ERR;
    unsigned long startMil = millis();
    String Comm = Command.substring(0,2);
    uint16_t value = Command.substring(2).toInt();
    if(Comm == COM_MEASAT_ROT)
    {
      
      if(abs(value - uNowPosR) > 1)
      {
          if(checkDeg(ID_SERVO_ROT,value))
          {
            servo.moveJoint(ID_SERVO_ROT,degToInt(value));
            int val = degToInt(value);
            int pos = servo.getJointPosition(ID_SERVO_ROT);
            int diff = abs(pos-degToInt(value));
            while(diff>C_SERVODIFF && millis() < startMil + TIMEOUT_T)
            {
                pos = servo.getJointPosition(ID_SERVO_ROT);
                diff = abs(pos-degToInt(value));
                delay(5);
            }
            if(millis() < startMil + TIMEOUT_T+500)
            {
                uNowPosR = value;
                tempsensor.measure(true);
                resp = stringTemps();  
            }

          }
      }
      else
      {
        tempsensor.measure(true);
        resp = stringTemps();

      }
    }
    else if (Comm == COM_MEASAT_TILT)
    {
      if(abs(value - uNowPosT) > 1)
      {
          if(checkDeg(ID_SERVO_TILT,value))
          {
            servo.moveJoint(ID_SERVO_TILT,degToInt(value));
            while(abs(servo.getJointPosition(ID_SERVO_TILT)-degToInt(value))>C_SERVODIFF && millis() < startMil + TIMEOUT_T)
            {
                delay(5);
            }
            if(millis() < startMil + TIMEOUT_T +500)
            {
                uNowPosR = value;
                tempsensor.measure(true);
                resp = stringTemps();
            }
          }
      }
      else
      {
            tempsensor.measure(true);
            resp = stringTemps();
      }
    }
    else if(Comm == COM_GOTO_ROT)
    {
      if(abs(value - uNowPosR) > 1)
      {
          if(checkDeg(ID_SERVO_ROT,value))
          {
            servo.moveJoint(ID_SERVO_ROT,degToInt(value));
            int val = degToInt(value);
            int pos = servo.getJointPosition(ID_SERVO_ROT);
            int diff = abs(pos-degToInt(value));
            while(diff>C_SERVODIFF && millis() < startMil + TIMEOUT_T)
            {
                pos = servo.getJointPosition(ID_SERVO_ROT);
                diff = abs(pos-degToInt(value));
                delay(5);
            }
            if(millis() < startMil + TIMEOUT_T+1000)
            {
                uNowPosR = value;
                resp = COM_ANSWER_OK;
            }
          }
      }
      else
      {
          resp = COM_ANSWER_OK;
      }
    }
    else if(Comm == COM_GOTO_TILT)
    {
      if(abs(value - uNowPosT) > 1)
      {
          if(checkDeg(ID_SERVO_TILT,value))
          {
            servo.moveJoint(ID_SERVO_TILT,degToInt(value));
            while(abs(servo.getJointPosition(ID_SERVO_TILT)-degToInt(value))>C_SERVODIFF && millis() < startMil + TIMEOUT_T)
            {
                delay(5);
            }
            if(millis() < startMil + TIMEOUT_T+1000)
            {
                uNowPosT = value;
                resp = COM_ANSWER_OK;
            }
          }
      }
      else
      {
          resp = COM_ANSWER_OK;
      }
       
    }
    else if(Comm == COM_MEASURE)
    {
        tempsensor.measure(true);
        resp = stringTemps();

    }
    else if(Comm == COM_STATUS)
    {
        resp = "OK";
    }
    else
    {
        resp = COM_ANSWER_ERR;
    }
    return resp;
}

uint16_t degToInt(uint16_t &deg)
{
    return (int) deg/300.*1023.;
}

bool checkDeg(uint8_t ID, uint16_t const &deg)
{
    bool bRetVal = true;
    if(ID == ID_SERVO_ROT)
    {
        if(deg> MAXDEG_ROT || deg < MINDEG_ROT)
            bRetVal = false;
    }
    else if(ID == ID_SERVO_TILT)
    {
        if(deg >  MAXDEG_TILT || deg < MINDEG_TILT)
            bRetVal = false;
    }
    else
    {
        bRetVal = false;
    }
    return bRetVal;
}

bool getCommFlag()
{
    return bNewCommandFlag;
}
void setCommFlag(bool flag)
{
    bNewCommandFlag = flag;
}

void setWebSocketConnectedFlag( bool state)
{
    webSocketConnectedFlag = state;
}

WiFiServer* getServer()
{
    return &server;
}

void wifiServerLoop()
{
    wm.process();
}