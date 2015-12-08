#include <CapacitiveSensor.h>
#include <Servo.h>
#include <EducationShield.h>
Knob pot=Knob(A5);//a knob is connected to A0

void setup(){
  //initialize the components
  pot.setLevels(10);
  
  Serial.begin(9600);
}

void loop(){
  int a = analogRead(A5);
  int b = pot.getLevel();
  
  Serial.print("Analog: ");
  Serial.print(a);
  Serial.print("  Level: ");
  Serial.println(b);
  
  delay(100);

}
