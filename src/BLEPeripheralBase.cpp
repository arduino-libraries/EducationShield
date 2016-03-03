#if defined(__arc__)

#include "EducationShield.h"


BLEPeripheralBase::BLEPeripheralBase(const char* serviceID):
	service(serviceID)
{

}

void BLEPeripheralBase::setName(const char* name){
	peri.setLocalName(name);
}


void BLEPeripheralBase::begin(){
	peri.setAdvertisedServiceUuid(service.uuid());
	peri.addAttribute(service);
}

bool BLEPeripheralBase::searchCentral(){
	BLECentral cen=peri.central();
	central=&cen;
	if(*central)
		return true;
	else
		return false;
}

bool BLEPeripheralBase::connected(){
	return peri.connected();
}

#endif