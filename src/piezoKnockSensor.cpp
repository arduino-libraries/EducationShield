#include "EducationShield.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

PiezoKnockSensor::PiezoKnockSensor(int pin){
	this->pin=pin;
}

void PiezoKnockSensor::config(int threshold, int debounceTime){
	this->threshold=threshold;
	this->debounceTime=debounceTime;
}

bool PiezoKnockSensor::knocked(int timeout){
	unsigned long begin=millis();
	do{
		if(analogRead(pin)>(unsigned long)threshold){
			delay(debounceTime);
			return true;
		}
	}while(millis()-begin>(unsigned long)timeout || timeout==0);
	return false;
}

void PiezoKnockSensor::test(){
	int val=analogRead(A5);
	Serial.println(val);
	delay(debounceTime);
}
