#include <helper_3dmath.h>
#include <MPU6050.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <MPU6050_6Axis_MotionApps_V6_12.h>
#include <MPU6050_9Axis_MotionApps41.h>

#include"xbee.h"
#include"motor.h"
#include"mpu.h"

#include<Servo.h>
#define PI 3.14159
#define encPin1 2     //SET PIN NUMBERS
#define encPin2 3
#define servoPin 4

XBee mod(&Serial1);  	//mention the name of serial being used to communicate with XBee
motor reaction(0,0,0);  //change the pin numbers here
motor drive(1,1,1);     //change the pin numbers here
Servo handle;
CompFil mpu;
int encoderCount=0,prevCount=0;
double theta,phi;
double dTheta,dPhi;

void setup()
{
  Serial2.begin(9600);
  handle.attach(servoPin);
  handle.write(0);
  attachInterrupt(digitalPinToInterrupt(encPin1),encoderHandler,RISING);
  pinMode(encPin2,INPUT_PULLUP);  
}

void loop()
{
	
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
/*******************ENABELING TIMER TO MEASURE RPM************************************* 
 * TIMER IS USED IN CTC MODE
 * OCR2A = 156 GENERATES INTERRUPT AFTER EVERY 10ms.
 * TIMER IS ENABLES WITH PRESCALER OF 1024.
 * TIMER INTERRUPT IS ENABLED.
 */
	noInterrupts();          // disable all interrupts
	TCCR2A = (1<<WGM21);     //CTC mode
	TCCR2B = 7;              //1024 prescaler
	OCR2A = 156;             // compare match register, setting for 10ms
	
	
	TIMSK2 = (1 << OCIE2A);  // enable timer compare interrupt

	TCNT2  = 0;

	interrupts();             // enable all interrupts
}

ISR(TIMER0_COMPA_VECT)
{}

ISR(TIMER2_COMPA_vect)          // timer compare interrupt service routine
{
	dPhi = (encoderCount - prevCount)*2*PI/ (280*0.01);	//in rad/sec
	prevCount=encoderCount;
}
