#include <Servo.h>

Servo myservo1, myservo2;

void setup() {
  myservo1.attach(9);
  myservo2.attach(5);
}

void loop() {
  myservo2.detach();
  myservo1.attach(9);
  myservo1.write(70);
  delay(1000);
  myservo1.write(120);
  delay(1000);

  myservo1.detach();
  myservo2.attach(5);
  myservo2.write(70);
  delay(1000);
  myservo2.write(120);
  delay(1000);
}

