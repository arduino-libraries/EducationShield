#if defined(__arc__)

#include "EducationShield.h"


BLEPeripheralBase::BLEPeripheralBase()/*const char* serviceID):
	service(serviceID)*/
{

}

void BLEPeripheralBase::setName(const char* name){
	BLE.setLocalName(name);
}


/*void BLEPeripheralBase::begin(){
	//peri.setAdvertisedService(service.uuid());
	//peri.addAttribute(service);

}*/

bool BLEPeripheralBase::searchCentral(){
	central=BLE.central();
	//central=&cen;
	if(central)
		return true;
	else
		return false;
}

bool BLEPeripheralBase::connected(){
	return central.connected();
}

#endif
