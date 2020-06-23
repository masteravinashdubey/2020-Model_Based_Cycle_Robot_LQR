
#include "I2Cdev.h"
//#include "MPU6050.h"
#include<Arduino.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include"DMP.h"
#include"xbee.h"
#include"motor.h"
#include"mpu.h"

#include<Servo.h>
#define PI 3.14159
#define encPin1 18     //SET PIN NUMBERS
#define encPin2 19
#define servoPin 9

//XBee mod(&Serial1);  	//mention the name of serial being used to communicate with XBee
motor reaction(30, 31, 3); //change the pin numbers here
motor drive(32, 33, 4);   //change the pin numbers here
#include"controller_lqr.h"
Servo handle;
CompFil mpu6050;
volatile int encoderCount = 0, prevCount = 0;
float previousRoll=0,angVelocity=0;
long prevtime = 0;

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

//  mpu6050.init();
MPUInit();
  Serial.println("going in loop");
  handle.attach(servoPin);
  handle.write(0);
  pinMode(encPin2, INPUT_PULLUP);
  pinMode(encPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encPin1), encoderHandler, RISING);

  enable_timer();


  Serial.println("ok");

}
bool check = 0, check2 = 0;


void loop()
{
  //Serial.println(check);
  //Serial.println(mpu.roll_deg);
  //  Serial.print("\t");
  //Serial.println(mpu.roll);
  //  Serial.print("Omega:\t");
  //  Serial.println(mpu.omega);

  if ((micros() - prevtime) >= 5)
  {
    //  Serial.println("going in lqr");
    lqr(ypr[0],angVelocity, phi, phidot);

    prevtime = micros();
  }
getDMP();
  if (check)
  {
//    mpu6050.read_accel();
//    mpu6050.read_gyro();
//    mpu6050.complimentary_filter_roll();
    // Serial.println(mpu.roll_deg);
    angVelocity=(ypr[0]-previousRoll)/0.003;
    previousRoll=ypr[0];
 
    check = 0;
  }
  if (check2)
  {
    phidot = (encoderCount - prevCount) * 2 * PI / (280 * 0.01); //in rad/sec
    prevCount = encoderCount;
    //Serial.println(phidot);
    check2 = 0;
  }
}

void encoderHandler()
{
  if (digitalRead(encPin2))
    encoderCount++;
  else
    encoderCount--;

  phi = encoderCount * 2 * PI / 270;
//  Serial.println(encoderCount);
//  if(phi>360)
//  encoderCount=0;
}

void enable_timer()
{
  noInterrupts();          // disable all interrupts

  TCCR2A = (1 << WGM21);   //CTC mode
  TCCR2B = 7;              //1024 prescaler
  OCR2A = sampleTime(5);             // compare match register, setting for 10ms
  TIMSK2 = (1 << OCIE2A);  // enable timer compare interrupt
  TCNT2  = 0;

  TCCR0A = (1 << WGM01);          //CTC mode
  TCCR0B = (1 << CS02) | (1 << CS00); //1024 prescaler
  OCR0A = sampleTime(3);                     // compare match register, setting for 3ms
  TIMSK0 = (1 << OCIE0A);         // enable timer compare interrupt
  TCNT0  = 0;

  interrupts();             // enable all interrupts
}
ISR(TIMER2_COMPA_vect)
{
  check2 = 1;

  //  PORTJ^=0x01;
}

ISR(TIMER0_COMPA_vect)
{
  check = 1;
  //  PORTJ^=0x02;
}
