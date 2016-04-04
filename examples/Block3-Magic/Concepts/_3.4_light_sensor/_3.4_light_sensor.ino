int ledPin=9;
int lightSensorPin=A1;
int potPin=A5;

void setup() {
  pinMode(ledPin,OUTPUT);
}

void loop() {
  int lightSensorValue=analogRead(lightSensorPin);
  int threshold=analogRead(potPin);

  if(lightSensorValue>threshold){
    digitalWrite(ledPin,LOW);
  }
  else{
    digitalWrite(ledPin,HIGH);
  }
  
  delay(10);
}

