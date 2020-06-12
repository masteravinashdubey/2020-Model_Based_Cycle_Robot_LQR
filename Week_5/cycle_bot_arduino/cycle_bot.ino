#include"xbee.h"
#include"motor.h"

void setup()
{
  XBee module(&Serial1);  //mention the name of serial being used to communicate with XBee
  motor reaction(0,0,0);  //change the pin numbers here
  motor drive(1,1,1);     //change the pin numbers here
  
}

void loop()
{
  // put your main code here, to run repeatedly:

}
