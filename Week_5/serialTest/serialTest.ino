#include"xbee.h"
#include"motor.h"
#include"mpu.h"

#include<Servo.h>
#define PI 3.14159
#define encPin1 2     //SET PIN NUMBERS
#define encPin2 3
#define servoPin 4

XBee mod(&Serial1);    //mention the name of serial being used to communicate with XBee
motor reaction(0,0,0);  //change the pin numbers here
motor drive(1,1,1);     //change the pin numbers here


void setup()
{
  Serial2.begin(9600);
  Serial.begin(9600);
}

void loop()
{
  Serial2.write("test");
  
  Serial.println(mod.read());
}
