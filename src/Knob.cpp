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

Knob::Knob(int pin){
	this->pin=pin;
}

int Knob::getValue(){
	return analogRead(this->pin);
}

void Knob::setLevels(int levels){
	this->levels=levels;
}
int Knob::getLevel(){
	int val=map(this->getValue(),0,1023,0,this->levels);
	if(val==this->levels)val--;
	return val;
}
