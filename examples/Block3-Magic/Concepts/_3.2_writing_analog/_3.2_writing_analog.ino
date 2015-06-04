int ledPin = 10;
int fade = 0;

void setup() {
  // nothing here
}

void loop() {
  analogWrite(ledPin, fade);
  delay(10);
  fade = fade + 10;
  if (fade > 255) fade = 0;
}

