#include <CapacitiveSensor.h>
#include <EducationShield.h>

#include <CurieImu.h>


IMU imu; // the IMU



int piezoPin = 9; //piezo

//Button variables
int inputPin = 10;
int buttonState = 0;
bool toggleState = false;
int previous = LOW;    // the previous reading from the input pin


const int ledPin = 13;      // activity LED pin
boolean blinkState = false; // state of the LED

//smoothing function
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;



void setup() {
  Serial.begin(9600); // initialize Serial communication


  imu.begin();



//smoothing function
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }


 
  pinMode(inputPin, INPUT);
   pinMode(piezoPin, OUTPUT);
  // configure Arduino LED for activity indicator
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // turn on led to show that the board has executed

}

void loop() {
  // start the IMU read
  imu.run();

  // read raw accel measurements from the IMU device
  int ay = imu.getAccelerometerY();

  // map the raw value to be on a smaller scale
  ay = map(ay, -36044 , 36043 , -360, 360);


  //Computes the absolute value of the IMU read
  int absoluteAy = abs(ay);

  // get the average for the ay measurement
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = absoluteAy;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits

  // map the average to a scale for the frequency for the piezo
  int frequency = map(average, 0, 360, 200, 700);

  //Check buttons
  buttonState = digitalRead(inputPin);

  if (buttonState  == HIGH && previous == LOW) {
    //toggle the button each time its pressed
    toggleState = !toggleState;
  }

  if (toggleState) {
    //play sound based on the average reading for the ay value
    tone(piezoPin, frequency);
  } else {
    //turn off the sound
    noTone(piezoPin);
  }

  //save the previous to make sure that the button has been on then off for the toggle to work
  previous = buttonState;


 // get the gyro result from the filter and convert them into INT
  int pitch = imu.getPitch();
  int roll = imu.getRoll();

  // when Serial is open, check for a 's' from Procssing
  if (Serial.available() > 0) {
    int val = Serial.read();
    if (val == 's') { // if incoming serial is "s"

      // print the roll,pitch and toggleState to the Procssing
      Serial.print(ay); Serial.print(",");

      Serial.print(pitch);  Serial.print(",");

      Serial.print(toggleState);  Serial.print(",");
      Serial.println("");

    }
  }


  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(ledPin, blinkState);


}


