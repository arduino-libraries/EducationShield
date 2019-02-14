/*  Serialportreceive
*
* connect a potentiometer to your board and you will check its value by using Processing
*
*       (c) 2013-2016 Arduino LLC.
*/

import processing.serial.*;

Serial myPort; // The serial port

void setup() {
  size(640, 480);



  // List all the available serial ports
  println(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[0], 9600);
}
void draw(){
 if (myPort.available() > 0) {
   
   //read from
    int inByte = myPort.read();
    println(inByte);
   }

}