/*Global Variables*/
float reqthetadot = 0, reqtheta = 0, reqphi = 0, reqphidot = 0;
float errorthetadot = 0, errortheta = 0, errorphi = 0, errorphidot = 0;

const float angle_offset = 1.8 ;   //1.8
int dummy = 0;
float U = 0, U_new = 0, U_previous = 0;
/******************/

void lqr(double roll, double angVelocity, double phi, double phidot)
{
  //Serial.println("in lqr");
  //Gain matrix optained from octave for sampling time of 7 ms
 
  
  double k[4] = {-51.918378  , -6.141140 ,  -0.084855 ,  -0.167508};    // { distance, velocity, angle, angular velocity }

  //  Serial.println(mpu.omega);
     // Serial.println(phidot);
  // When traversing in the normal arena

  // k[1] += k[0];
  //k[0] = 0;
  reqtheta = 0;
  reqthetadot = 0;
  reqphi = 0;
  reqphidot = 0;

  roll = roll * 180 / M_PI;
  //angVelocity = angVelocity * 180 / M_PI;
//   if(phi>=360)phi=0;
// Serial.println(roll);
  //Serial.println(angVelocity);
   //Serial.println(phi);
   //Serial.println(phidot);

  //Serial.print("\t");
  errortheta = (roll - reqtheta) - angle_offset ;    //Error in the distance covered
  //  Serial.print(errortheta);
  //  Serial.print("\t");
  errorthetadot = (angVelocity - reqthetadot);           //Error in the velocity
  errorphi = (phi - reqphi) ;                         //Error in the tilt angle
  errorphidot = (phidot - reqphidot);                 //Error in angular velocity

  U = (-(k[0] * errortheta) - (k[1] * errorthetadot) - (k[2] * errorphi) - (k[3] * errorphidot));
  //  U=U-U_previous;
  //  U_previous=U;
  U_new = constrain(U*255/12 , -255, 255);

// Serial.print(U);
// Serial.print("\t");
//   Serial.println(U_new);
  //  if(U_new >= 0)
  //  {
  //    reaction.setDir(1);
  //    if(abs(U_new) >255)
  //    {
  //      reaction.setPWM(255);
  //    }
  //    reaction.setPWM(abs(U_new));
  //  }
  //  else
  //  {
  //    reaction.setDir(1);
  //    if(abs(U_new) >255)
  //    {
  //      reaction.setPWM(255);
  //    }
  //    reaction.setPWM(abs(U_new));
  //  }
  // Update the PWM  and direction
  reaction.setTheSpeed(U_new);

//  reaction.setTheSpeed(U);

//  Q = [50 0 0 0;
//     0 280 0 0;                          % Q matrix of system
//     0 0 1 0;
//     0 0 0 60];

// Q = [60 0 0 0;
//     0 160 0 0;                          % Q matrix of system
//     0 0 10 0;
//     0 0 0 30];

}
