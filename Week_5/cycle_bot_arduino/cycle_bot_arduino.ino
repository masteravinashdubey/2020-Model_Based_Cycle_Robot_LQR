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
  Serial2.begin(9600);
  Serial.begin(115200);
  handle.attach(servoPin);
  handle.write(0);
  attachInterrupt(digitalPinToInterrupt(encPin1),encoderHandler,RISING);
  pinMode(encPin2,INPUT_PULLUP);
  enable_timer();
  mpu.init();
}

void loop()
{
//  Serial.print(mpu.roll_deg);
//  Serial.print("\t");
//  Serial.print(mpu.roll);
//  Serial.print("Omega:\t");
//  Serial.println(mpu.omega);

 if ((micros() - prevtime) >= 7000)
  {
    lqr();
    prevtime = micros();
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
/*******************ENABELING TIMER TO MEASURE RPM************************************* 
 * TIMER IS USED IN CTC MODE
 * OCR2A = 156 GENERATES INTERRUPT AFTER EVERY 10ms.
 * TIMER IS ENABLES WITH PRESCALER OF 1024.
 * TIMER INTERRUPT IS ENABLED.
 */
	noInterrupts();          // disable all interrupts
	TCCR2A = (1<<WGM21);     //CTC mode
	TCCR2B = 7;              //1024 prescaler
	OCR2A = 78;             // compare match register, setting for 5ms
	
	
	TIMSK2 = (1 << OCIE2A);  // enable timer compare interrupt

	TCNT2  = 0;

	//interrupts();             // enable all interrupts
	
	//TIMER0
	
	//noInterrupts();          // disable all interrupts
	TCCR0A = (1<<WGM01);     //CTC mode
	TCCR0B = 5;              //1024 prescaler
	OCR0A = 46;             // compare match register, setting for 3ms
	
	
	TIMSK0 = (1 << OCIE0A);  // enable timer compare interrupt

	TCNT0  = 0;

	interrupts();             // enable all interrupts
}

ISR(TIMER0_COMPA_VECT)
{
 phidot = (encoderCount - prevCount)*2*PI/ (280*0.01); //in rad/sec
  prevCount=encoderCount;
   Serial.println(phidot);
}

ISR(TIMER2_COMPA_vect)          // timer compare interrupt service routine
{
	  Serial.println("ok");
mpu.testing();
//Serial.println("done");   
 // mpu.read_accel();
//    Serial.println("1");
 //mpu.read_gyro();
 //Serial.println("2");
 mpu.complimentary_filter_roll();
//Serial.println("3");
}
