int speakerPin = 8;

void setup() {
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  digitalWrite(speakerPin, HIGH);
  delay(1);
  digitalWrite(speakerPin, LOW);
  delay(1);
}

