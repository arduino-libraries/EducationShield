
#include <EducationShield.h>


IMU imu; // the IMU

Button button(6);
int buttonState = 0;


const int ledPin = 13;      // activity LED pin
boolean blinkState = false; // state of the LED

void setup() {
  Serial.begin(9600); // initialize Serial communication

  imu.begin();
  button.begin();

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
  if(button.isPressed()){
    buttonState=true;
  }

  // when Serial is open, send a "s" to processing
  if (Serial.available() > 0) {
    int val = Serial.read();
    if (val == 's') { // if incoming serial is "s"
      Serial.print(roll); Serial.print(",");


      Serial.print(buttonState);  Serial.print(",");
      Serial.println("");

      buttonState=0;

    }
  }




  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(ledPin, blinkState);
}
