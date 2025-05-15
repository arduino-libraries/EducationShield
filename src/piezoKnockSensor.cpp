/*
	Copyright (C) Arduino SA

	This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
	distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "EducationShield.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

PiezoKnockSensor::PiezoKnockSensor(int pin) : Button(pin,HIGH){
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
	}while(millis()-begin<(unsigned long)timeout || timeout==0);
	return false;
	//return pressed(timeout);
}

void PiezoKnockSensor::test(){
	int val=analogRead(A5);
	Serial.println(val);
	delay(debounceTime);
}
