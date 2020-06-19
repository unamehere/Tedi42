#include "mainFunc.h"
#include "../pinmap.h"
#include "../constants.h"
#include "../lib/XL320/XL320.h"
#include <SoftwareSerial.h>

//globals
MLX90621 tempsensor;
XL320 servo;
uint16_t uNowPosR;
uint16_t uNowPosT;

String Command;
bool bNewCommandFlag;


bool m_init()
{
    bool retVal = true;
    bNewCommandFlag = false;
    Command = "";
    Command.reserve(COM_MAXLEN);
    Serial.begin(C_COMBAUD);
    retVal = initTemp();
    retVal = initServo();
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
    //toDo
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
            if (x<15) Serial.print(",");
        }
        if(y<3) Serial.print(";");
    }
    Serial.println(";TE");
}

void handleNewCommand()
{
    
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
            if(millis() < startMil + TIMEOUT_T)
            {
                uNowPosR = value;
                tempsensor.measure(true);
                serialSendTemps();;
            }
            else
            {
                Serial.println(COM_ANSWER_ERR);
            }
          }
          else
          {
              Serial.println(COM_ANSWER_ERR);
          }
      }
      else
      {
        tempsensor.measure(true);
        serialSendTemps();
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
            if(millis() < startMil + TIMEOUT_T)
            {
                uNowPosR = value;
                tempsensor.measure(true);
                serialSendTemps();
            }
            else
            {
                Serial.println(COM_ANSWER_ERR);
            }
            

          }
          else
          {
              Serial.println(COM_ANSWER_ERR);
          }
      }
      else
      {
            tempsensor.measure(true);
            serialSendTemps();
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
            if(millis() < startMil + TIMEOUT_T)
            {
                uNowPosR = value;
                Serial.println(COM_ANSWER_OK);
            }
            else
            {
                Serial.println(COM_ANSWER_ERR);
            }
          }
          else
          {
              Serial.println(COM_ANSWER_ERR);
          }
      }
      else
      {
          Serial.println(COM_ANSWER_OK);
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
            if(millis() < startMil + TIMEOUT_T)
            {
                uNowPosT = value;
                Serial.println(COM_ANSWER_OK);
            }
            else
            {
                Serial.println(COM_ANSWER_ERR);
            }
          }
        else
          {
              Serial.println(COM_ANSWER_ERR);
          }
      }
      else
      {
          Serial.println(COM_ANSWER_OK);
      }
       
    }
    else if(Comm == COM_MEASURE)
    {
        tempsensor.measure(true);
        serialSendTemps();
    }
    else if(Comm == COM_STATUS)
    {
        Serial.print("Hello ");
        Serial.println(COM_ANSWER_OK);
    }
    else
    {
        Serial.println(COM_ANSWER_ERR);
    }
    Command = "";
}

uint16_t degToInt(uint16_t &deg)
{
    return (int) deg/300.*1024.;
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

String* getCommand()
{
    return &Command;
}
