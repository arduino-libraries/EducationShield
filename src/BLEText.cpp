#include "EducationShield.h"

BLEText::BLEText():
	BLEPeripheralBase("19b10000-e8f2-537e-4f6c-d104768a1214"),
	textChari("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite, MAX_LENGTH)
{

}


void BLEText::begin(){
	BLEPeripheralBase::begin();
	peri.addAttribute(textChari);

	peri.begin();
}

bool BLEText::dataReceived(){
	return textChari.written();
}

char BLEText::getCharAt(int position){
	return (char)textChari.value()[position];

}
