
/*Global Variables*/
float reqthetadot = 0, reqtheta = 0, reqphi = 0, reqphidot = 0;
float errorthetadot = 0, errortheta = 0, errorphi = 0, errorphidot = 0;
double theta,phi;
double thetadot,phidot;
const float angle_offset = (0.01745 * 3);
int dummy = 0;
float U = 0, U_new = 0;
/******************/

void lqr()
{
  //Gain matrix optained from octave for sampling time of 7 ms
  float k[4] = {-25.801483, -3.048010, -0.066327, -0.082783 };     // { distance, velocity, angle, angular velocity }

  {
    // When traversing in the normal arena         
    {
      k[1] += k[0];
      k[0] = 0;
      reqtheta = 0;
      reqthetadot = 0;
      reqphi = 0;
      reqphidot = 0;
    }
  }

  errortheta = (theta - reqtheta) + angle_offset ;    //Error in the distance covered
  errorthetadot = (thetadot - reqthetadot);           //Error in the velocity
  errorphi = (phi - reqphi) ;                         //Error in the tilt angle
  errorphidot = (phidot - reqphidot);                 //Error in angular velocity

  U = ( - k[0] * errortheta - k[1] * errorthetadot - k[2] * errorphi - k[3] * errorphidot);
  U_new = constrain(U * 255 / 12 , -255, 255);

  {
    // Update the PWM  and direction
    reaction.setSpeed(U_new);
  }
}
