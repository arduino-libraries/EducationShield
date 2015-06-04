void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A1);
  
  Serial.println(sensorValue);
  delay(100);
}

