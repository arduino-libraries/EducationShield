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

UltrasonicSensor::UltrasonicSensor(int trig, int echo){
	this->trig=trig;
	this->echo=echo;
}

int UltrasonicSensor::getDistance(){
	pinMode(trig, OUTPUT);
	digitalWrite(trig,LOW);
	delayMicroseconds(2);
	digitalWrite(trig, HIGH);
	delayMicroseconds(5);
	digitalWrite(trig, LOW);
	
	pinMode(echo, INPUT);
	int duration = pulseIn(echo, HIGH,11800);
	int distance = duration/29/2;
	
	return distance;
}
