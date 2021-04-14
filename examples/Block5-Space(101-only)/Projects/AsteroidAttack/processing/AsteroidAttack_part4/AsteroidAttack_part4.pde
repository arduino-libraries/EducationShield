/*
* AsteroidAttack
* 
* You will create a retro style space
* shooter game, and learn about how to use classes and functions. In the end you will be able to
* tweak values and rewrite the program to redesign the game so it suits your taste.
*
* (c) 2013-2016 Arduino LLC.
*/

import processing.serial.*;

// ArrayLists where we can put our objects
ArrayList<Star> starArr = new ArrayList<Star>(); // Array of stars
ArrayList<Shot> shotArr = new ArrayList<Shot>(); // Array of shots

// Serial communication
Serial myPort; // Serial port variable
int newLine = 13; // New line character in ASCII
float angleVal; // Stores the incoming angle value
int buttonState; // Stores the incoming button state
String message; // String that stores all incoming data

String [] valArray = new String [2]; // Array to store all incoming values

// Star variables
float nbrOfStars=40; // Number of stars
float starVal; // Used as star counter

// Timer variables
float timeSinceStart;

float shotTimer; // Shot timer
float lastShotCheck; // Recording last check
float shotInterval=100; // The interval between shots, in milliseconds

//player variables
float playerWidth=100; // Width of the player
float playerHeight=20; // Height of the player
float playerX; // Player x position
float playerY; // Player y position
float posVar=0;   // Variable for angle based position

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

  // We write an 's' to receive data from Arduino
  myPort.write("s"); 

  // Create stars
  while (starVal<nbrOfStars) {
    createNewStar();
    starVal++;
  }
}

void draw()
{
  // We read the incoming serial message
  serialEvent();  

  // We update the visuals
  updateVisuals(); 

  // We want the following changes to apply to the same matrix only
  pushMatrix();

  // Update player position
  playerPos();

  // Update player rotation
  playerRotation(); 

  // Draw the player
  drawPlayer();

  popMatrix();

  // Game logic
  gameController();
} 

void serialEvent() {
  message = myPort.readStringUntil(newLine); // Read from port until new line (ASCII code 13)
  if (message != null) {
    valArray = split(message, ","); // Split message by commas and store in String array 
    angleVal = float(valArray[0]); // Convert to float angleVal
    buttonState = int(valArray[1]); // Convert to int buttonState

    myPort.write("s"); // Write an "s" to receive more data from the board
  }
}

void playerPos() {

  //Add position every frame
  posVar+=angleVal;

  //Position the player avatar
  playerX=width/2+posVar; // Set center as the player's starting x position
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
  //We add the current
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

  createStars();
  createShots();
}

void createNewStar() {
  //We add a new star to the star array
  starArr.add(new Star(random(0, width), random(0, height), random(1, 4)));
}

void createStars() {

  translate(0, 0);

  //We create a for loop that loops through all stars
  for (int i=0; i<starArr.size(); i++) {
    //We create a local instance of the star object
    Star star = starArr.get(i);
    star.move(angleVal);
    star.display();
  }
}

void newShot() {
  //We add a new star to the shot array
  shotArr.add(new Shot(playerX, playerY));
}

void createShots() {

  translate(0, 0);

  // A for loop that loops through all shots
  for (int j=0; j<shotArr.size(); j++) {
    // We create a local instance of the shot object
    Shot shot = shotArr.get(j);
    shot.show();
  }
}

void gameController() {

  // If the button is pressed and the shotTimer variable has reached the interval
  if (buttonState==1&&shotTimer>shotInterval) {
    newShot(); // We add a new shot
    lastShotCheck=timeSinceStart; // We save the current time since start
  } else {
    shotTimer=0; // The timer is reset
  }

  timeSinceStart=millis(); // Assign current time
  shotTimer=timeSinceStart-lastShotCheck; // Assign time since last shot
}