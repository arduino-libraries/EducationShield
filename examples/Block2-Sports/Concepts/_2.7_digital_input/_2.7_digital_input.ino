int inputPin = 5;
int ledPin = 13;

void setup() {
  pinMode(inputPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (digitalRead(inputPin) == HIGH) {
    digitalWrite(ledPin, HIGH);
  } 
  else {
    digitalWrite(ledPin, LOW);
  }
}

