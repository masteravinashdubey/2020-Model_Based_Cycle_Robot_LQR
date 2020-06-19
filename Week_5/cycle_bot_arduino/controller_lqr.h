
/*Global Variables*/
float reqVelocity = 0, reqDistance = 0, reqRoll = 0, reqOmega = 0;
float errorVelocity = 0, errorDistance = 0, errorRoll = 0, errorOmega = 0;
const float angle_offset = (0.01745 * 3);
int dummy = 0;
float U = 0, U_new = 0;
/******************/

void lqr()
{
  //Gain matrix optained from octave for sampling time of 7 ms
  float k[4] = {-25.801483, -3.048010, -0.066327, -0.082783 };
                               
  {
    // When traversing in the normal arena         
    {
      k[1] += k[0];
      k[0] = 0;
      reqDistance = 0;
      reqVelocity = 0;
      reqRoll = 0;
      reqOmega = 0;
    }
  }
  
  errorDistance = (distance - reqDistance);           //Error in the distance covered
  errorVelocity = (velocity - reqVelocity);           //Error in the velocity
  errorRoll = (roll - reqRoll) + angle_offset ;       //Error in the tilt angle
  errorOmega = (omega - reqOmega);                    //Error in angular velocity

  U = ( - k[0] * errorDistance - k[1] * errorVelocity - k[2] * errorRoll - k[3] * errorOmega);
  U_new = constrain(U * 255 / 12 , -255, 255);

  {
    // Update the PWM  and direction
    moveMotor(U_new, U_new);
  }
}
