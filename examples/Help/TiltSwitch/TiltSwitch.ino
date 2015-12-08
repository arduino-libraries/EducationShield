/*  TiltSwitch
*
*  Tilt Switch is a component that detects if it’s right side up
*  or tilted.
*
*  (c) 2013 Arduino Verkstad
*/

#include <CapacitiveSensor.h>
#include <Servo.h>
#include <EducationShield.h>

//Declare the tilt switch. A TinkerKit tilt switch can be
//connected to D5 or D9 (We use D9 here).
TiltSwitch me=TiltSwitch(9);

void setup(){
  Serial.begin(9600);

  //Initialize the component. Must be called.
  me.begin();
}
void loop(){
  Serial.println("Please tilt...");

  //pressed(timeout)
  //	timeout: milliseconds within which the tilt switch should
  //		be tilted. If tilted within time, the function
  //		returns 1, otherwise 0. If not used, it waits
  //		untill being pressed.
  //
  //Wait until the tilt switch is tilted.
  Serial.println(me.pressed());

}
