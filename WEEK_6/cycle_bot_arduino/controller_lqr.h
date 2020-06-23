/*Global Variables*/
float reqthetadot = 0, reqtheta = 0, reqphi = 0, reqphidot = 0;
float errorthetadot = 0, errortheta = 0, errorphi = 0, errorphidot = 0;
double theta, phi;
double thetadot, phidot;
const float angle_offset = (0.01745 * 3);
int dummy = 0;
float U = 0, U_new = 0;
/******************/

void lqr(double roll,double angVelocity, double phi, double phidot)
{
  //Serial.println("in lqr");
  //Gain matrix optained from octave for sampling time of 7 ms
  float k[4] = { -27.204 , -3.2097 , -0.079934 , -0.088978 };    // { distance, velocity, angle, angular velocity }
  //Serial.print("roll= ");
  //  Serial.println(mpu.roll_deg);
  //  Serial.print("omega= ");
  //  Serial.println(mpu.omega);
//     Serial.println(angVelocity);
  // When traversing in the normal arena

  // k[1] += k[0];
  //k[0] = 0;
  reqtheta = 0;
  reqthetadot = 0;
  reqphi = 0;
  reqphidot = 0;


  errortheta = (roll - reqtheta); //+ angle_offset ;    //Error in the distance covered
  //  Serial.print(errortheta);
  //  Serial.print("\t");
  errorthetadot = (angVelocity - reqthetadot);           //Error in the velocity
  errorphi = (phi - reqphi) ;                         //Error in the tilt angle
  errorphidot = (phidot - reqphidot);                 //Error in angular velocity

  U = (-1 * (k[0] * errortheta) - (k[1] * errorthetadot) - (k[2] * errorphi) - (k[3] * errorphidot));
  U_new = constrain(U * 255 / 12 , -255, 255);
  //U_new = U * 255/12;
 // Serial.println(U);
//  Serial.println(U_new);
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

}
