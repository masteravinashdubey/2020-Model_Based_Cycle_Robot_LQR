class XBee
{
    byte adc0LSB = 0, adc0MSB = 0;
    byte adc1LSB = 0, adc1MSB = 0;
    byte adc2LSB = 0, adc2MSB = 0;
    byte adc3LSB = 0, adc3MSB = 0;
    byte digitalLSB = 0;
    byte digitalMSB = 0;
    Stream *serial;

  public:

    int leftX = 0;
    int leftY = 0;
    int rightX = 0;
    int rightY = 0;

    XBee(Stream *usingSerial)
    {
      serial = usingSerial;
      if (usingSerial == &Serial) Serial.begin(9600);
      else if (usingSerial == &Serial1) Serial1.begin(9600);
      else if (usingSerial == &Serial2) Serial2.begin(9600);
      else if (usingSerial == &Serial3) Serial3.begin(9600);
    }

    void readPacket()
    {
      if (serial->available() > 29)
      {
        if (serial->read() == 0x7E)
        {
          for (int i = 0; i <= 18; i++)
            byte disc = serial->read();

          digitalMSB = serial->read();
          digitalLSB = serial->read();

          adc0MSB = serial->read();
          adc0LSB = serial->read();

          adc1MSB = serial->read();
          adc1LSB = serial->read();

          adc2MSB = serial->read();
          adc2LSB = serial->read();

          adc3MSB = serial->read();
          adc3LSB = serial->read();

          leftX = adc0MSB << 8 | adc0LSB;
          leftY = adc1MSB << 8 | adc1LSB;
          rightX = adc2MSB << 8 | adc2LSB;
          rightY = adc3MSB << 8 | adc3LSB;
        }
      }
    }

    byte read()
    {
      return serial->read();
    }
};
