#include "mainFunc.h"
#include "../pinmap.h"
#include "../constants.h"

MLX90621 tempsensor;

bool m_init()
{
    bool retVal = true;
    Serial.begin(C_COMBAUD);
    retVal = initTemp();
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
    //toDo
    return retVal;
}

void serialSendTemps()
{
    float* temps = tempsensor.getAllTemps();
    Serial.print("TS");
    for(int y = 0; y < T_ROWCOUNT; y++)
    {
        for(int x = 0; x <T_COLUMNCOUNT; x++)
        {
            Serial.print(temps[y+x*4]);
            if (x<15) Serial.print(",");
        }
        if(y<3) Serial.print(";");
    }
    Serial.println("TE");
}

void m_measure()
{
    tempsensor.measure(true);
}

