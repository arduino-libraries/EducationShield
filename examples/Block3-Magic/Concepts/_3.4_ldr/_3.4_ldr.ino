int ledPin=10;
int ldrPin=A1;
int potPin=A5;

void setup() {
  pinMode(ledPin,OUTPUT);
}

void loop() {
  int ldrValue=analogRead(ldrPin);
  int threshold=analogRead(potPin);

  if(ldrValue>threshold){
    digitalWrite(ledPin,LOW);
  }
  else{
    digitalWrite(ledPin,HIGH);
  }
  
  delay(10);
}

