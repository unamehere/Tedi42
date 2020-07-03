#ifndef CONSTANTS
#define CONSTANTS

#define TIMEOUT_T 2000

#define T_ROWCOUNT 4
#define T_COLUMNCOUNT 16

#define C_COMBAUD 115200
#define C_SERVOBAUD 9600

#define C_SERVOSPEED 512
#define C_SERVODIFF 10

#define MAXDEG_ROT 300
#define MINDEG_ROT 0
#define MAXDEG_TILT 290
#define MINDEG_TILT 155

#define ID_SERVO_ROT 2
#define ID_SERVO_TILT 1

#define COM_MAXLEN 6
#define COM_GOTO_ROT        "GR"
#define COM_GOTO_TILT       "GT"
#define COM_MEASAT_ROT      "MR"
#define COM_MEASAT_TILT     "MT"
#define COM_MEASURE         "MM"
#define COM_STATUS          "ST"

#define COM_ANSWER_OK       "OK"
#define COM_ANSWER_ERR      "ER"

#define COM_SOCKET_CONNECT  "SC"
#define COM_SOCKET_IP       "SI"



#endif