
#ifndef TEMPMEASURE
#define TEMPMEASURE
#include "../MLX90621/MLX90621.h"
#include "../pinmap.h"

/*
Sensor is measuring following Data
   0   1   2   3
0  x   x   x   x
1  x   x   x   x
2  x   x   x   x
3  x   x   x   x
4  x   x   x   x
5  x   x   x   x
6  x   x   x   x
7  x   x   x   x
8  x   x   x   x
9  x   x   x   x
10 x   x   x   x
11 x   x   x   x
12 x   x   x   x
13 x   x   x   x
14 x   x   x   x
15 x   x   x   x
16 x   x   x   x

*/

#define PIXEL_ROWS 16
#define PIXEL_COLUMMS 4

class tempmeasurement
{
private:
    MLX90621 sensor;
    uint16_t tempVals[PIXEL_ROWS * PIXEL_COLUMMS];
public:
    tempmeasurement();
    ~tempmeasurement();
    uint16_t* getVals();
    bool newTemps();

};







#endif