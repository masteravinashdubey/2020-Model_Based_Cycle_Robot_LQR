#include"xbee.h"
#include"motor.h"

XBee mod(&Serial1);  //mention the name of serial being used to communicate with XBee
motor reaction(0,0,0);  //change the pin numbers here
motor drive(1,1,1);     //change the pin numbers here
void setup()
{
  
  Serial2.begin(9600);
  
  
}

void loop()
{
	Serial2.println("test");
	Serial.print(mod.read());

}
