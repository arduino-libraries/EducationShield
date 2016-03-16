/*
 * Nova Beam
 *
 * Create an augmented reality application that gives you the opportunity
 * to play with a nova beam. 
 *
 * (c) 2016 M. Ehrndal, Arduino Verkstad, Sweden
 */


import processing.video.*;
import processing.serial.*;

Capture video; // Object of the Capture class

// Serial communication
Serial myPort; // Serial port variable
int newLine = 13; // New line character in ASCII
int buttonState; // Stores the incoming button state
float rollVal; // Stores the incoming roll value
float inVal; // Value to store incoming rollVal after mapping
float pitchVal; // Stores the incoming pitch value
String message; // String that stores all incoming data

String [] valArray = new String [0]; // Array to store all incoming values

// Laser sword variables
float imgWidth=350; // Image size
float laserHeight=800; // Image size
float adjustedY; // Position y
boolean laserOn=false; // Variable to set state
float laserGlowSize=300; // The size of the uplit area 
float lightposx; // Light x position
float lightposy; // Light y position

String[] imgFiles = {"laserSwordHandle.png", "laserSwordBeam.png"}; // Array of image names
PImage[] img = new PImage[2]; // PImage object array

void setup() {
  fullScreen();
  smooth();
  rectMode(CORNERS);

  // List all the available serial ports
  println(Serial.list());

  /*Set my serial port to same as the board, baud rate 9600*/
  myPort = new Serial(this, Serial.list()[0], 9600); 

  // We write an 's' to receive data from Arduino
  myPort.write("s");

  // Setup the camera
 // initializeCamera(0);

  // Load the images
  for (int i = 0; i < 2; i = i + 1) {
    img[i] = loadImage(imgFiles[i]);
  }
}

void draw() {
  // Read the incoming serial message
  serialEvent();   

  // Handle video processing
  videoHandler();

  // Draw the laser sword
  drawSword();
}

void serialEvent() {
  message = myPort.readStringUntil(newLine); // read from port until new line (ASCII code 13)
  if (message != null) {
    valArray = split(message, ","); // split message by commas and store in String array 
    rollVal = float(valArray[0]); // convert to float rollVal
    pitchVal = float(valArray[1]); // convert to float pitchVal
    buttonState= int(valArray[2]); // convert to int buttonState

    myPort.write("s"); // write an "s" to receive more data from Arduino

    // If statement to constrain values within certain range
    if (rollVal>-90&&rollVal<90) {
      // Map to constrain values and avoid movement jitter  
      inVal=map(rollVal, 90, -90, 50, -50);
    }
    // If statement to constrain values within certain range
    if (pitchVal>-90&&pitchVal<10) {
      //Map to adapt range to laser sword and window height 
      laserHeight=map(pitchVal, 90, -90, 0, 1200);
    }
  }
}

void drawSword() {

  calculateLight();

  pushMatrix();

  // We place the image in the middle, and at suitable height 
  translate(width/2, height);

  // We rotate the image
  rotate(radians(inVal));

  /*We move the image back so that the rotation pivot 
   is aligned in the center and bottom of the handle*/

  // We move up the handle 110 units
  translate(-imgWidth/2, -110);

  // Sword handle image
  image(img[0], 0, 0, imgWidth, 110); 

  // Visually adjust the laser beam
  adjustedY=laserHeight-10;

  // We move up the beam 380 units
  translate(0, -adjustedY);

  // Only draw laser beam when it is toggled "ON"
  if (buttonState==1) {
    // Laser width is constant, but height is dynamic 
    image(img[1], 0, 0, imgWidth, laserHeight);
  }

  popMatrix();
}

void videoHandler() {


    background(50);
  
}

void calculateLight() {

  // Calculate x position for light based on window size and incoming value
  lightposx=width/2+(inVal*3);

  // Calculate y position for light based on laser center and visual result
  lightposy=480/2;
}

void initializeCamera(int camNum) {
  String[] cameras = Capture.list();

  if (cameras.length == 0) {
    println("There are no cameras available for capture.");
    exit();
  } else {
    println("Available cameras:");
    for (int i = 0; i < cameras.length; i++) {
      println("["+i+"] "+cameras[i]);
    }

    // The camera can be initialized directly using an element
    // from the array returned by list():
    video = new Capture(this, width, height, cameras[camNum]);
    video.start();
  }   

  // we need the camera at full blast
  frameRate(30);
}