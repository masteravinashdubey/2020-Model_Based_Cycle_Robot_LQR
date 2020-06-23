class motor
{
    byte pwmPin;
    byte dirPin1, dirPin2;
    bool dir = 0;
    byte pwm = 0;
    const int deadbandRight = 45, deadbandLeft = 45;

  public:
    motor(byte dirPin1, byte dirPin2, byte pwmPin)
    {
      this->dirPin1 = dirPin1;
      this->dirPin2 = dirPin2;
      this->pwmPin = pwmPin;
      pinMode(dirPin1, OUTPUT);
      pinMode(dirPin2, OUTPUT);
      pinMode(pwmPin, OUTPUT);
    }

    void setDir(bool d)
    {
      dir = d;
      digitalWrite(dirPin1, d);
      digitalWrite(dirPin2, !d);
    }

    void setPWM(byte p)
    {
      pwm = p;
      analogWrite(pwmPin, pwm);
    }

    void setTheSpeed(int mSpeed)
    {

      digitalWrite(dirPin1, mSpeed <= 0);
      digitalWrite(dirPin2, mSpeed > 0);
      if (mSpeed < 0)
        analogWrite(pwmPin, map(abs(mSpeed), 0, 255, deadbandLeft, 255));
      else
        analogWrite(pwmPin, map(abs(mSpeed), 0, 255, deadbandRight, 255));
    }
};
