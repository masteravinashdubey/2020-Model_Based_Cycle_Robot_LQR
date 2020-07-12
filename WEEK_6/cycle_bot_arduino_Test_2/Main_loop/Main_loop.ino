//#define F_CPU 16000000
#include "I2Cdev.h"
//#include "MPU6050.h"
#include<Arduino.h>
#include <TimerOne.h>
#include <TimerThree.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include"DMP.h"
#include"xbee.h"
#include"motor.h"
#include"mpu.h"

//#include<Servo.h>
#define PI 3.14159
#define encPin1 18     //SET PIN NUMBERS
#define encPin2 9
#define servoPin 10

//XBee mod(&Serial1);    //mention the name of serial being used to communicate with XBee
motor reaction(30, 31, 7); //change the pin numbers here
motor drive(32, 33, 8);   //change the pin numbers here
#include"controller_lqr.h"
//Servo handle;
//CompFil mpu6050;
volatile int encoderCount = 0, prevCount = 0;
float previousRoll = 0, angVelocity = 0;
long prevtime = 0;
volatile double  phi=0,previousPhi=0;
volatile double  phidot=0;

char sampleTime(long Time)
{
  return (Time * 16000 / 1024);
}
void setup()
{

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  Serial.begin(115200);

  //   mpu6050.init();
  MPUInit();
  Serial.println("going in loop");
  //  handle.attach(servoPin);
  // handle.write(0);
  pinMode(encPin2, INPUT_PULLUP);
  pinMode(encPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encPin1), encoderHandler, RISING);

  enable_timer();


  Serial.println("ok");

}
volatile bool check = 0, check2 = 0;


void loop()
{
  //Serial.println(check);
  //Serial.println(mpu.roll_deg);
  //  Serial.print("\t");
  //Serial.println(mpu.roll);
  //  Serial.print("Omega:\t");
  //  Serial.println(mpu.omega);
  //
    if ((millis() - prevtime) >= 5)
    {
             phi = encoderCount * 1.33;
//  Serial.print(encoderCount);
//  Serial.print("\t");
  
    phidot = (phi-previousPhi)/(millis() - prevtime)/1000; //in rad/sec
    
   
 
       if(phi>=360)encoderCount=0;

   previousPhi=phi;
  
      prevtime = millis();
      Serial.print(phi);
         Serial.print("\t");
    Serial.println(phidot);
    }
  getDMP();
  if (check)
  {

    check = 0;
    //    mpu6050.read_accel();
    //    mpu6050.read_gyro();
    //    mpu6050.complimentary_filter_roll();
    // Serial.println(mpu.roll_deg);
    angVelocity = (ypr[2] - previousRoll) / 0.005;
    previousRoll = ypr[2];
//       phi = encoderCount * 1.33;
////  Serial.print(encoderCount);
////  Serial.print("\t");
////  Serial.print(phi);
//    phidot = (phi-previousPhi)/0.005; //in rad/sec
//    
////   
////    Serial.print("\t");
////    Serial.println(phidot);
//       if(phi>=360)encoderCount=0;
//
//   previousPhi=phi;
    lqr(ypr[2], angVelocity, phi,  phidot);
    //    lqr(mpu6050.roll, angVelocity, phi, phidot);

  }
  if (check2)
  {
    check2 = 0;
//   phi = encoderCount * 1.33;
////  Serial.print(encoderCount);
////  Serial.print("\t");
////  Serial.print(phi);
//    phidot = (phi-previousPhi)/0.003; //in rad/sec
//    
////   
////    Serial.print("\t");
////    Serial.println(phidot);
//       if(phi>=360)encoderCount=0;
//
//   previousPhi=phi;

  }
}

void encoderHandler()
{
  if (digitalRead(encPin2))
    encoderCount++;
  else
    encoderCount--;


//  phi = encoderCount * (360 / 270);
//  Serial.print(encoderCount);
//  Serial.print("\t");
//  Serial.println(phi);
  //  if(phi>360)
  //  encoderCount=0;
}

void enable_timer()
{
  Timer1.initialize(5000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timer1Isr ); // attach the service routine here
  Timer3.initialize(3000);
  Timer3.attachInterrupt( timer3Isr );
}
void timer1Isr()
{
  check = 1;
}
void timer3Isr()
{
  check2 = 1;
}
