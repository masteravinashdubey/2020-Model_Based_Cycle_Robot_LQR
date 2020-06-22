class CompFil
{
	int16_t ax, ay, az, gx, gy, gz, gnx, gny, gnz;
	float a[3] = {0, 0, 0}, g[3] = {0, 0, 0};

	const float pi = 3.14159265359, f_cut = 5, dT = 0.003, comp_alpha = 0.01;
	float Tau = 1 / (2 * pi*f_cut); 	// Time constant
	float alpha = Tau / (Tau + dT);
	

	const float accel_sf = 16384, gyro_sf = 131;	//scaling factor for accelerometer and gyroscope
	float lpx = 0, lpy = 0, lpz = 0, hpx = 0, hpy = 0, hpz = 0;
	bool m = 1, n = 1;
	
	MPU6050 mpu;

	void set_offsets()
	{
//  Serial.println("in set offset"); 
		// Offsets calculated by putting gy-87 on a flat surface for 5 minutes
		mpu.setXAccelOffset(-4636);   // Set offset for X acceleration
		mpu.setYAccelOffset(1106);    // Set offset for Y acceleration
		mpu.setZAccelOffset(2694);    // Set offset for Z acceleration

		mpu.setXGyroOffset(261);     //Set offset for X gyro
		mpu.setYGyroOffset(-34);       //Set offset for Y gyro
		mpu.setZGyroOffset(0);      //Set offset for Z gyro
	}

	/*
	 * Function Name: low_pass_filter
	 * Input: accelerometer values in 3 axis
	 * Output: None
	 * Logic: To filter the accelerometer readings
	 * Example Call: low_pass_filter(a[0], a[1], a[2]);
	 */ 
	void low_pass_filter(float Ax, float Ay, float Az)
	{

		if (n)
		{
			lpx = (1 - alpha) * Ax;    //Low Pass filtered AccelX
			lpy = (1 - alpha) * Ay;    //Low Pass filtered AccelY
			lpz = (1 - alpha) * Az;    //Low Pass filtered AccelZ
			n++;
		}
		else
		{
			lpx = (1 - alpha) * Ax + alpha * lpx;
			lpy = (1 - alpha) * Ay + alpha * lpy;
			lpz = (1 - alpha) * Az + alpha * lpz;
		}
	}

	/*
	 * Function Name: high_pass_filter
	 * Input: gyroscope values in 3 axis
	 * Output: None
	 * Logic: To filter the gyroscope readings
	 * Example Call: high_pass_filter(g[0], g[1], g[2]);
	 */
	void high_pass_filter(float Gx, float Gy, float Gz)
	{

		if (m)
		{
			hpx = (1 - alpha) * Gx;     //High Pass filtered GyroX
			hpy = (1 - alpha) * Gy;     //High Pass filtered GyroY
			hpz = (1 - alpha) * Gz;     //High Pass filtered GyroZ
			m++;
		}
		else
		{
			hpx = (1 - alpha) * (hpx + Gx - gnx);
			hpy = (1 - alpha) * (hpy + Gy - gny);
			hpz = (1 - alpha) * (hpz + Gz - gnz);
		}
		gnx = Gx;
		gny = Gy;
		gnz = Gz;
	}
public:
	float roll_deg = 0;
	float roll = 0, omega = 0;

	void init()
	{
	// join I2C bus (I2Cdev library doesn't do this automatically)
 
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif



    // initialize device
    Serial.println("Initializing I2C devices...");
    mpu.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
	}

	/*
 * Function Name: read_accel
 * Input: None
 * Output: None
 * Logic: Reads the acceleration along X,Y,Z axes
 * Example Call: read_accel()
 */ 

void read_accel()
{
//  Serial.println("in read accel");
  mpu.getAcceleration(&ax, &ay, &az); //To get raw acceleration values 

  a[0] = (ax / accel_sf);                   //Divide by scaling factor
  a[1] = (ay / accel_sf); 
  a[2] = (az / accel_sf);
  low_pass_filter(a[0], a[1], a[2]);
}

/*
 * Function Name: read_gyro
 * Input: None
 * Output: None
 * Logic: To read the angular velocity from Gyroscope
 * Example Call: read_gyro()
 */
void read_gyro()
{
//  Serial.println("in read gyro");
  mpu.getRotation(&gx, &gy, &gz);          //To get raw gyro values 
  g[0] = (gx / gyro_sf);                   //Divide by scaling factor
  g[1] = (gy / gyro_sf);
  g[2] = (gz / gyro_sf);
  high_pass_filter(g[0], g[1], g[2]);
}

/*
 * Function Name: complimentary_filter_roll
 * Input: None
 * Output: None
 * Logic: Fuse the accel ang gyro values
 * Example Call: complimentary_filter_roll()
 */
void complimentary_filter_roll()
{
//  Serial.println("in read comp filter");
  // Compute the roll angle by fusing angles from accel and gyro 
  roll_deg = (1 - comp_alpha) * (roll_deg + g[2] * dT) + (comp_alpha) * (atan(a[1] / abs(a[0]))) * (180 / 3.14); 
  // Convert the angle to radians
  roll = roll_deg * (3.14 / 180);
  // Calculate angular velocity 
  omega = g[1] * (pi / 180);        
}
void testing()
{
  Serial.println("test done");
}
};	
