void setup() {
  Serial.begin(9600);//Default baudrate of XBee is 9600, so this is also set as 9600.

}

void loop() {
  Serial.write("Sending Successful!\n");  //write the message after every 2 seconds.
  delay(2000);

}
