#include <avr/io.h>
#include <avr/interrupt.h>
#include<Servo.h>

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

Servo handle;
bool s0=1,s2=1;
int main(void)
{
  enable_timer();
  handle.attach(8);
//  DDRJ=255;
  pinMode(14,1);
  pinMode(15,1);
    /* Replace with your application code */
    while (1) 
    {
      for(int i=0;i<90;i++)
      {
        handle.write(i);
        delay(50);
      }
      for(int i=90;i>0;i--)
      {
        handle.write(i);
        delay(50);
      }
    }
}

ISR(TIMER2_COMPA_vect)
{
  if(s2)
  {
    digitalWrite(15,1);
    s2=0;
  }
  else
  {
    digitalWrite(15,0);
    s2=1;
  }
  //  PORTJ^=0x01;
}

ISR(TIMER0_COMPA_vect)
{
  if(s0)
  {
    digitalWrite(14,1);
    s0=0;
  }
  else
  {
    digitalWrite(14,0);
    s0=1;
  }
//  PORTJ^=0x02;
}

