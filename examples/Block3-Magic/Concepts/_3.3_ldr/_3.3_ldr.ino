int ledPin=9;
int ldrPin=A1;

void setup() {
  //nothing here
}

void loop() {
  int ldrValue=analogRead(ldrPin);
  int ledValue=map(ldrValue,0,1023,0,255);

  analogWrite(ledPin, ledValue);
  delay(10);
}

