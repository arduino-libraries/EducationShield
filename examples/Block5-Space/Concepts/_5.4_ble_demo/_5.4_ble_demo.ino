#include <EducationShield.h>
#include <CurieBle.h>

BLEuart ble=BLEuart(TYPE_MESSENGER);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ble.setName("Messenger");
  ble.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(ble.searchCentral()){
    Serial.println("Connected to central ");
    while(ble.connected()){

      //When receiving data from mobile app, write it 
      //back to the sender
      if(ble.dataReceived()){
        ble.fetchData();
        Serial.println(ble.receivedString());
        char data[20];
        int length=ble.getReceivedLength();
        for(int i=0;i<length;i++){
          data[i]=ble.receivedString()[i];
        }
        ble.sendString(data,length);
      }

    }
    Serial.println(F("Disconnected from central "));

  }
}
