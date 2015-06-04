int speakerPin = 8;

void setup() {
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  digitalWrite(speakerPin, HIGH);
  delay(1000);
  digitalWrite(speakerPin, LOW);
  delay(1000);
}

