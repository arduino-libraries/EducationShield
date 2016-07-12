/*
*  _5.8_translate_demo
* 
*
* In this example, you will get a clearer demonstration of how the
* translate() method works in relation to pushMatrix() and popMatrix(). 
* 
* (c) 2013-2016 Arduino LLC.
*/

pushMatrix(); //Pushmatrix

fill(150);
translate(20, 20);
rect(0, 0, 20, 20);  // Grey rectangle

fill(255);      
translate(40, 20); //50, 50
rect(0, 0, 20, 20);  //White rectangle

popMatrix(); //Popmatrix

translate(40, 20); //50, 50
rect(0, 0, 20, 20);  //White rectangle