#include <EducationShield.h>

BLEuart ble=BLEuart();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Curie BLE setup
  // set advertised local name
  ble.setName("CutomControl");
  ble.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(ble.searchCentral()){
    Serial.println("Connected to central ");
    while(ble.connected()){

      //If data is sent through BLE to 101 board
      if(ble.dataReceived()){
        //Fetch all data from BLE
        ble.fetchData();

        //Read the 1 byte data received
        unsigned char data=ble.getValueAt(0);
        Serial.println(data);
      }
    }
    Serial.println(F("Disconnected from central "));

  }
}
