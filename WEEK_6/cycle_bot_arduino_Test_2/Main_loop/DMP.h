


#define INTERRUPT_PIN 19  // use pin 2 on Arduino Uno & most boards


/*MPU Init*/

    MPU6050 mpu;
    volatile bool dmpReady = false;  // set true if DMP init was successful
    uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
    uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer
    Quaternion q;           // [w, x, y, z]         quaternion container
    VectorInt16 aa;         // [x, y, z]            accel sensor measurements
    VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
    VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
    VectorFloat gravity;    // [x, y, z]            gravity vector
    float euler[3];         // [psi, theta, phi]    Euler angle container
    float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
    uint8_t teapotPacket[14] = { '$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n' };

    // ================================================================
    // ===               INTERRUPT DETECTION ROUTINE                ===
    // ================================================================

    volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

     void dmpDataReady() {
      mpuInterrupt = true;
    }
     void MPUInit()
    {
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
       Wire.setClock(400000);
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
#endif
      Serial.println(F("Initializing I2C devices..."));
      mpu.initialize();
      pinMode(INTERRUPT_PIN, INPUT);

      // verify connection
      Serial.println(F("Testing device connections..."));
      Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
      Serial.println(F("Initializing DMP..."));
      devStatus = mpu.dmpInitialize();

      mpu.setXAccelOffset(-3024);   // Set offset for X acceleration
      mpu.setYAccelOffset(1108);    // Set offset for Y acceleration
      mpu.setZAccelOffset(828);    // Set offset for Z acceleration

      mpu.setXGyroOffset(258);     //Set offset for X gyro
      mpu.setYGyroOffset(-34);       //Set offset for Y gyro
      mpu.setZGyroOffset(1);      //Set offset for Z gyro

      delay(500);
      if (devStatus == 0)
      {
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);
        Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
        Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
        Serial.println(F(")..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
      }
    }
    void getDMP()
    {
      if (!dmpReady) return;

      // wait for MPU interrupt or extra packet(s) available
      while (!mpuInterrupt && fifoCount < packetSize) {
        if (mpuInterrupt && fifoCount < packetSize) {
          // try to get out of the infinite loop
          fifoCount = mpu.getFIFOCount();
        }
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
      }

      // reset interrupt flag and get INT_STATUS byte
      mpuInterrupt = false;
      mpuIntStatus = mpu.getIntStatus();

      // get current FIFO count
      fifoCount = mpu.getFIFOCount();
      if (fifoCount < packetSize) {
        //Lets go back and wait for another interrupt. We shouldn't be here, we got an interrupt from another event
        // This is blocking so don't do it   while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
      }
      // check for overflow (this should never happen unless our code is too inefficient)
      else if ((mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        //  fifoCount = mpu.getFIFOCount();  // will be zero after reset no need to ask
        Serial.println(F("FIFO overflow!"));

        // otherwise, check for DMP data ready interrupt (this should happen frequently)
      } else if (mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {

        // read a packet from FIFO
        while (fifoCount >= packetSize) { // Lets catch up to NOW, someone is using the dreaded delay()!
          mpu.getFIFOBytes(fifoBuffer, packetSize);
          // track FIFO count here in case there is > 1 packet available
          // (this lets us immediately read more without waiting for an interrupt)
          fifoCount -= packetSize;
        }

      }
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity); 
//      Serial.print("ypr\t");
//      Serial.print(ypr[0] * 180 / M_PI);
//      Serial.print("\t");
//      Serial.print(ypr[1] * 180 / M_PI);
//      Serial.print("\t");
      //Serial.println(ypr[2] * 180 / M_PI);

    }
