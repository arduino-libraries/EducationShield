/*
*  _5.9_rotate_demo
* 
*
* In this example, you will get a clearer demonstration of how the rotate() method works.  
* 
* (c) 2013-2016 Arduino LLC.
*/

float size=25;

pushMatrix(); //Pushmatrix

translate(width/2, height/2); //Move the coordinate system to the center
rotate(radians(45)); //Rotate the rectangle 45 degrees
translate(-size/2, -size/2); //Center the rectangle

rect(0, 0, size, size);

popMatrix(); //Popmatrix