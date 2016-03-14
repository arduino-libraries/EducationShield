
#include <EducationShield.h>
#include <CurieBle.h>

BLEuart uart=BLEuart(TYPE_TAMAGOTCHI);
IMU imu;

// led pins
const int redLed = 12;
const int greenLed = 11;
const int yellowLed = 10;

const int lightPin = A0; //phototransistor

//Button variables
const int inputPin = 9;
int buttonState = 0;
int previous = LOW;    // the previous reading from the input pin


int exerciseLv, foodLv, sleepLv; // the saved stats that will go down over time

long lastTimer; //used for saveing millis()
bool updated; // checks if the stats need to be updated or not


void setup() {
  Serial.begin(9600);

  // configure Arduino LED for activity indicator
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH); // turn on led to show that the board has executed


  // Curie IMU setup
  //--------------------------------------------------
  /* Initialise the IMU */
  imu.begin();
  imu.detectShock();
  imu.attachCallback(eventCallback);
  // Curie IMU setup end
  //--------------------------------------------------



  // Curie BLE setup
  //--------------------------------------------------
  // set advertised local name and service UUID:
  uart.setName("Tamagotchi");
  uart.begin();
  // Curie BLE setup end
  //--------------------------------------------------



  // starting levels for the three states
  exerciseLv = 100;
  foodLv = 100;
  sleepLv = 100;

  // configure the 2 leds to be outputs
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  // sets the timer to the corrent millis() count
  lastTimer = millis();
}

void loop() {

  updateStatus(); // used to update the stats values

  // if a central is connected to peripheral:
  if(uart.searchCentral()){
    Serial.println("Connected to central ");
    // while the central is still connected to peripheral:
    while(uart.connected()){
      updateStatus();// used to update the stats values
      if (updated) { // when the stats are updated send the updated value to the connected device
        Serial.println("Sending data");
        uart.addValue((unsigned char)exerciseLv);
        uart.addValue((unsigned char)foodLv);
        uart.addValue((unsigned char)sleepLv);    
        uart.send();
        
        updated=false;
      }
    }
    // when the central disconnects, print it out:
    Serial.println(F("Disconnected from central "));

  }

  // if any stats are to low, light the leds with the fitting color

  if (sleepLv < 5) {
    digitalWrite(redLed, HIGH);
  } else {
    digitalWrite(redLed, LOW);
  }
  if (foodLv < 5) {
    digitalWrite(greenLed, HIGH);
  } else {

    digitalWrite(greenLed, LOW);
  }
  if (exerciseLv < 5) {
    digitalWrite(yellowLed, HIGH);
  } else {
    digitalWrite(yellowLed, LOW);
  }






}

void updateStatus() {


  // read the current light levels from the phototransistor
  int lightValue = analogRead(lightPin);

  // if the light level is low, add 1 to the sleepLv
  if (lightValue < 200) {
    sleepLv++;
    if (sleepLv > 100) sleepLv = 100;
  }

  buttonState = digitalRead(inputPin);
  if (buttonState == HIGH && previous == LOW) { //button is pushed
    foodLv++;
    if (foodLv > 100) foodLv = 100;
  }



  // every 3 sec update and lower the 3 stats in diffrent rates
  if (millis() - lastTimer >= 3000) {
    if (exerciseLv > 0) {
      exerciseLv = exerciseLv - 1;
    } else {
      exerciseLv = 0;
    }

    if (foodLv > 0) {
      foodLv = foodLv - 2;
    } else {
      foodLv = 0;
    }
    if (sleepLv > 0) {
      sleepLv = sleepLv - 3;
    } else {
      sleepLv = 0;
    }
    updated = true;
    lastTimer = millis();
  }


  previous = buttonState; // sets the buttonstate to the previous

}

// this is called when the Imu detects  a shock in any direction
static void eventCallback(void)
{
  //add 1 to the exerciseLv
  exerciseLv++;
  if (exerciseLv > 100) exerciseLv = 100;
}
