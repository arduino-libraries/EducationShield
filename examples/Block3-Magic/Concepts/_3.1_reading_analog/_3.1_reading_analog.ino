int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int val = analogRead(A5);
  
  digitalWrite(ledPin, HIGH);
  delay(val);
  digitalWrite(ledPin, LOW);
  delay(val);
}

