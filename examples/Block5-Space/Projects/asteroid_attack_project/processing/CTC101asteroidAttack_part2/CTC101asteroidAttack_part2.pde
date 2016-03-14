/*
 * Asteroid attack
 *
 * Create a videogame using Processing. The game for this exercise is
 * getting your programming skills a little further and making a computer
 * game where your space craft, powered by Genuino 101, will help you 
 * become a winner. You learn how to use classes, objects and functions.
 *
 * Step 1:
 *  - Create a representation for the player
 *  - Receive values through serial communication
 *  - Use incoming value to rotate and move the player
 *  - Limit the players movement to the window
 *    
 *
 * (c) 2016 M. Ehrndal, Arduino Verkstad, Sweden
 */

import processing.serial.*;

// Serial communication
Serial myPort; // Serial port variable
int newLine = 13; // New line character in ASCII
float angleVal; // Stores the incoming angle value
int buttonState; // Stores the incoming button state
String message; // String that stores all incoming data

String [] valArray = new String [2]; // Array to store all incoming values

//player variables
float playerWidth=100; // Width of the player
float playerHeight=20; // Height of the player
float playerX; // Player x position
float playerY; // Player y position
float posVar=0; // Variable for angle based position

void setup()
{
  size(800, 600);
  smooth();
  fill(180);
  noStroke();

 // List all the available serial ports
  println(Serial.list());

  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[2], 9600); 

  //We write an 's' to receive data from the board
  myPort.write("s"); 
}

void draw() {
    //We read the incoming serial message
    serialEvent();  

    //We update the visuals
    updateVisuals(); 

    //We want the following changes to apply to the same matrix only
    pushMatrix();

    //Update player position
    playerPos();

    //Update player rotation
    playerRotation(); 

    //Draw the player
    drawPlayer();
    
    popMatrix();
} 

void serialEvent() {
  message = myPort.readStringUntil(newLine); // Read from port until new line (ASCII code 13)
  if (message != null) {
    valArray = split(message, ","); // Slit message by commas and store in String array 
    angleVal = float(valArray[0]); // Convert to float angeVal
    buttonState = int(valArray[1]); // Convert to int buttonState
  
    myPort.write("s"); // Write an "s" to receive more data from the board

  }
}

void playerPos() {
  
  //Add position every frame
  posVar+=angleVal;
  
  //Position the player avatar
  playerX=width/2+posVar; // Set center as the players starting x position
  playerY=height-100; // Set the y position of the player

  // Limit the X coordinates
  if (playerX <= 0) {
    playerX = 0;
  }
  if (playerX > width) {  
    playerX = width;
  }
 
  translate(playerX, playerY); 
}

void playerRotation() {
  //We add the current rotation
  rotate(radians(angleVal));
}

void drawPlayer() {

  rectMode(CENTER);
  fill(200);
  
  //Draw the player
  rect(0, 0, playerWidth, playerHeight);    
 
}

void updateVisuals() {
  background(40);
}