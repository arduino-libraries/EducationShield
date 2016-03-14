
#include <EducationShield.h>

BLEuart uart=BLEuart();

int count;
bool btnPressed;

void setup() {
  Serial.begin(9600);

  //Set pin modes of digital pins
  pinMode(9,INPUT);

  // Curie BLE setup
  // set advertised local name
  uart.setName("valDisp");
  uart.begin();


  count=0;
  btnPressed=false;
}

void loop() {
  // if a central is connected to peripheral:
  if(uart.searchCentral()){
    Serial.println("Connected to central ");
    // while the central is still connected to peripheral:
    while(uart.connected()){
      //Get readings from sensors 
      int buttonValue=digitalRead(9);
      int lightValue=analogRead(A1);

      //Value Display can only pass numbers smaller than 255
      lightValue=map(lightValue,0,1023,0,255);

      //Check if the button is being pressed, and update the
      //counter accordingly
      if(btnPressed==false && buttonValue==HIGH){
        count++;
        btnPressed=true;
      }else if(btnPressed==true && buttonValue==LOW){
        btnPressed=false;
      }


      //Note the sequence of data being sent, the phone app displays 
      //the values in the same squence.
      uart.addValue(buttonValue);
      uart.addValue(count);
      uart.addValue(lightValue);    
      uart.send();
        
    }
    // when the central disconnects, print it out:
    Serial.println(F("Disconnected from central "));

  }

}

