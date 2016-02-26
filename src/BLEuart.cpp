#include "EducationShield.h"

BLEuart::BLEuart():
	BLEPeripheralBase("6E400001-B5A3-F393-E0A9-E50E24DCCA9E"),
	txChari("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLENotify | BLERead, MAX_LENGTH),
	rxChari("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWrite, MAX_LENGTH)
{

}

void BLEuart::begin(){
	BLEPeripheralBase::begin();
	peri.addAttribute(txChari);
	peri.addAttribute(rxChari);

	peri.begin();

}

bool BLEuart::dataReceived(){
	return rxChari.written();
}

void BLEuart::fetchData(){
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

void BLEuart::addValue(unsigned char val){
	addValueAt(val,writeLength++);
}

void BLEuart::addValueAt(unsigned char val, int position){
	writeBuffer[position]=val;
}

unsigned char BLEuart::getValueAt(int position){
	return readBuffer[position];
}