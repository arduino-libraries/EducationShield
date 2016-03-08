#if defined(__arc__)

#include "EducationShield.h"

/*
BLEuart::BLEuart(const char* serviceID):
	BLEPeripheralBase(serviceID),
	txChari("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLENotify | BLERead, MAX_LENGTH),
	rxChari("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWrite, MAX_LENGTH),
	typeChari("6E400004-B5A3-F393-E0A9-E50E24DCCA9E", BLERead, MAX_LENGTH)
{

}*/
BLEuart::BLEuart(int exampleID):
	BLEPeripheralBase("6E400001-B5A3-F393-E0A9-E50E24DCCA9E"),
	txChari("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLENotify | BLERead, MAX_LENGTH),
	rxChari("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWrite, MAX_LENGTH),
	typeChari("6E400004-B5A3-F393-E0A9-E50E24DCCA9E", BLERead, MAX_LENGTH)
{
	setExampleID(exampleID);
}	

void BLEuart::begin(){
	BLEPeripheralBase::begin();
	peri.addAttribute(txChari);
	peri.addAttribute(rxChari);
	peri.addAttribute(typeChari);

	peri.begin();
}

void BLEuart::setExampleID(int exampleID){
	const unsigned char out[1]={(unsigned char)exampleID};
	typeChari.setValue(out,1);
}

bool BLEuart::dataReceived(){
	return rxChari.written();
}

void BLEuart::fetchData(){
	receivedLength=rxChari.valueLength();
	memcpy(readBuffer,rxChari.value(),rxChari.valueLength());
	readBuffer[rxChari.valueLength()]=0;
}
void BLEuart::send(){
	txChari.setValue(writeBuffer,writeLength);
	writeLength=0;
}

void BLEuart::sendString(const char* text,int length){
	txChari.setValue((unsigned char*)text,length);
}

const char* BLEuart::receivedString(){
	return (char*)readBuffer;
}

int BLEuart::getReceivedLength(){
	return (int)receivedLength;
}

void BLEuart::addValue(int val){
	addValueAt((unsigned char)val,writeLength++);
}

void BLEuart::addValueAt(unsigned char val, int position){
	writeBuffer[position]=val;
}

unsigned char BLEuart::getValueAt(int position){
	return readBuffer[position];
}

#endif