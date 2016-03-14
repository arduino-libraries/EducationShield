#include <EducationShield.h>
#include <CurieBle.h>

BLEuart uart=BLEuart(TYPE_MESSENGER);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  uart.setName("Messenger");
  uart.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(uart.searchCentral()){
    Serial.println("Connected to central ");
    while(uart.connected()){

      //When receiving data from mobile app, write it 
      //back to the sender
      if(uart.dataReceived()){
        uart.fetchData();
        Serial.println(uart.receivedString());
        char data[20];
        int length=uart.getReceivedLength();
        for(int i=0;i<length;i++){
          data[i]=uart.receivedString()[i];
        }
        uart.sendString(data,length);
      }

    }
    Serial.println(F("Disconnected from central "));

  }
}
