
#include <EducationShield.h>


IMU imu; // the IMU

//Button variables
int inputPin = 6;
int buttonState = 0;


const int ledPin = 13;      // activity LED pin
boolean blinkState = false; // state of the LED

void setup() {
  Serial.begin(9600); // initialize Serial communication



  imu.begin();


  pinMode(inputPin, INPUT);
  // configure Arduino LED for activity indicator
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // turn on led to show that the board has executed
}

void loop() {

  // read raw gyro measurements from device
  imu.run();


  // get the gyro result from the filter and convert them into INT
  int pitch = imu.getPitch();
  int roll = imu.getRoll();

  //Check buttons
  if (digitalRead(inputPin) == HIGH) {
    buttonState = 1;

  }
  else {
    buttonState = 0;

  }

  // when Serial is open, send a "s" to processing
  if (Serial.available() > 0) {
    int val = Serial.read();
    if (val == 's') { // if incoming serial is "s"
      Serial.print(roll); Serial.print(",");


      Serial.print(buttonState);  Serial.print(",");
      Serial.println("");


    }
  }




  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(ledPin, blinkState);
}
