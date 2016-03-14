#include "EducationShield.h"
#include <Arduino.h>

LightSensor::LightSensor(int pin):Button(pin,HIGH){
}

void LightSensor::calibrate(int t){
	unsigned long startT=millis();
	//int low=1023;
	int high=0;

	while(millis()-startT<(unsigned long)t){
		int val=analogRead(pin);
		/*if(val<low){
			low=val;
		}else if(val>high){
			high=val;
		}
		delay(2);*/
		if(val>high){
			high=val;
		}
	}

	this->base=high;
	this->threshold=high-100;
	//this->threashold=(high-low)/2;

}

void LightSensor::config(int baseValue, int threshold){
	//Base value should be the one when nothing is covering
	//Threashold should be between base and covering value

	this->base=baseValue;
	this->threshold=threshold;
}

void LightSensor::showConfig(){
	Serial.print("base "+this->base);
	Serial.print(this->base);
	Serial.print(' ');
	Serial.print("threashold ");
	Serial.println(this->threshold);
}

void LightSensor::test(){
	//Use the test to determin base and threashold first
	Serial.println(analogRead(pin));
}

bool LightSensor::getState(){
	int value=analogRead(pin);
	
	//return ((value-base)>(threashold-base)) || ((base-value)>(base-threashold));
	if(base>threshold){
		return value<threshold;
	}else{
		return value>threshold;
	}
}
