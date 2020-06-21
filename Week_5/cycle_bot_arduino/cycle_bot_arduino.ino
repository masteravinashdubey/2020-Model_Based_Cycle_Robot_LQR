#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include"xbee.h"
#include"motor.h"
#include"mpu.h"


#include<Servo.h>
#define PI 3.14159
#define encPin1 18     //SET PIN NUMBERS
#define encPin2 19
#define servoPin 9

XBee mod(&Serial1);  	//mention the name of serial being used to communicate with XBee
motor reaction(4,5,10);  //change the pin numbers here
motor drive(6,7,11);     //change the pin numbers here
Servo handle;
CompFil mpu;
int encoderCount=0,prevCount=0;
double theta,phi;
double thetadot,phidot;
long prevtime = 0;

#include"controller_lqr.h"

void setup()
{


  Serial.begin(115200);
  
 mpu.init();
  Serial.println("going in loop");
  handle.attach(servoPin);
  handle.write(0);
  attachInterrupt(digitalPinToInterrupt(encPin1),encoderHandler,RISING);
  pinMode(encPin2,INPUT_PULLUP);
  enable_timer();
  

 Serial.println("ok");

}
bool check = 0,check2=0;
void loop()
{
  //Serial.println(check);
//  Serial.print(mpu.roll_deg);
//  Serial.print("\t");
  Serial.println(mpu.roll);
//  Serial.print("Omega:\t");
//  Serial.println(mpu.omega);

 if ((micros() - prevtime) >= 7000)
  {
    lqr();
    prevtime = micros();
  }

  if(check)
  {
 mpu.read_accel(); 
 mpu.read_gyro();
 mpu.complimentary_filter_roll();
 check=0;
  }

}

void encoderHandler()
{
	if(digitalRead(encPin2))
		encoderCount++;
	else
		encoderCount--;

	phi = encoderCount*2*PI/280;
}

void enable_timer()
{
  noInterrupts();          // disable all interrupts
  
  TCCR2A = (1<<WGM21);     //CTC mode
  TCCR2B = 7;              //1024 prescaler
  OCR2A = 156;             // compare match register, setting for 10ms
  TIMSK2 = (1 << OCIE2A);  // enable timer compare interrupt
  TCNT2  = 0;

  TCCR0A = (1<<WGM01);            //CTC mode
  TCCR0B = (1<<CS02)|(1<<CS00);   //1024 prescaler
  OCR0A = 46;                     // compare match register, setting for 3ms
  TIMSK0 = (1 << OCIE0A);         // enable timer compare interrupt
  TCNT0  = 0;
  
  interrupts();             // enable all interrupts
}
ISR(TIMER2_COMPA_vect)
{
  check=1;

  //  PORTJ^=0x01;
}

ISR(TIMER0_COMPA_vect)
{
  check2=1;
  Serial.print(check2);
//  PORTJ^=0x02;
}
