void setup() {
  Serial.begin(9600);//Default baudrate of XBee is 9600, so this is also set as 9600.

}

void loop() {
  if(Serial.available())
  Serial.print(Serial.read());    //print whatever is received on the serial monitor.
}
