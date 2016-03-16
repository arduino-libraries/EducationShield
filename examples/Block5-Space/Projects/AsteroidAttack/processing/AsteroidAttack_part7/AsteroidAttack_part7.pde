/*
 * Asteroid attack
 *
 * Create a videogame using Processing. The game for this exercise is
 * getting your programming skills a little further and making a computer
 * game where your space craft, powered by Genuino 101, will help you 
 * become a winner
 *
 * Step 6:
 *  - We add images to the sketch
 *
 * (c) 2016 M. Ehrndal, Arduino Verkstad, Sweden
 */

import processing.serial.*;

//Arraylists where we can put our objects
ArrayList<Star> starArr = new ArrayList<Star>(); // Array of stars
ArrayList<Shot> shotArr = new ArrayList<Shot>(); // Array of shots
ArrayList<Asteroid> asteroidArr = new ArrayList<Asteroid>(); // Array of asteroids

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

//Shot variables
float shotX; // Shot x position
float shotY; // Shot y position
float shotSize; // Shot size

//Asteroid variables
float asteroidX; // Asteroid position x
float asteroidY; // Asteroid position y
float asteroidSize; // Asteroid size
float asteroidSpeed=2; // Asteroid speed
float asteroidMinSize=30; // Min size for random method
float asteroidMaxSize=80; // Max size for random method

//Game state variables
int points=0; // Point variable
PFont pointText; // Text for displaying points
boolean gameOver=false; // Boolean keeps track of game state

// Timer variables
float timeSinceStart; // Time since start

float shotTimer; // Shot timer
float lastShotCheck; // Recording last check
float shotInterval=100; // The interval between shots, in milliseconds

float asteroidTimer; // Asteroid timer
float lastAsteroidCheck; // Recording last check
float asteroidInterval=1000; // The interval between asteroids, in milliseconds

//player variables
float playerWidth=200; // Width of the player
float playerHeight=100; // Height of the player
float playerX; // Player x position
float playerY; // Player y position
float posVar=0; 
boolean playerHit=false; // Use to check if player is hit
float life=200; // The players life points

//Image array
String[] imgFiles={"spaceShip.png", "meteor_small.png", "meteor_big.png"};
PImage[] img = new PImage[3];

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
  
    // Create a font
  pointText = createFont("CourierNewPSMT-48.vlw", 48);
  textFont(pointText);
  
    // Load the images
  for(int i = 0; i < 3; i = i + 1) {
    img[i] = loadImage(imgFiles[i]);
  }
}

void draw()
{
  
    // If statement to check whether the player still has life
    if (life>0) {

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

    //We make sure to check if any shot hit any asteroid every frame
    checkHits();
    
    //Check whether player collided with an asteroid
    checkDamage();

    // Show points on screen
    drawPoints();

    gameController();
  } else {
    // Game over method is run when life is out
    gameOver();
  }
} 

void serialEvent() {
  message = myPort.readStringUntil(newLine); // Read from port until new line (ASCII code 13)
  if (message != null) {
    valArray = split(message, ","); // Split message by commas and store in String array 
    angleVal = float(valArray[0]); // Convert to float angeVal
    buttonState = int(valArray[1]); // Convert to int buttonState

    myPort.write("s"); // Write an "s" to receive more data from the board
  }
}

void playerPos() {

  // Add position every frame
  posVar+=angleVal;

  // Center player position in window
  playerX=width/2+posVar;
  playerY=height-100;

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

  //Represent the player with an image
  image(img[0], 0-playerWidth/2, 0-playerHeight/2, playerWidth, playerHeight); 
}

void updateVisuals() {

  if (playerHit==true) {
    background(random(100, 250), 100, 100); // If the player is hit we make the background flash in red
  } else {
    background(40);
  }

  createStars();
  createShots();
  createAsteroids();
  createLifeBar();
}

void createNewStar() {
  //We add a new star to the star array
  starArr.add(new Star(random(0, width), random(0, height), random(1, 4)));
}

void createStars() {

  translate(0, 0);

  // We create a for loop that loops through all stars
  for (int i=0; i<starArr.size(); i++) {
    //We create a local instance of the star object
    Star star = starArr.get(i);
    star.move(angleVal);
    star.display();
  }
}

void newShot() {
  // We add a new shot to the shot array
  shotArr.add(new Shot(playerX, playerY));
}

void createShots() {

  translate(0, 0);
  // A for loop that loops through all shots  
  for (int j=0; j<shotArr.size(); j++) {

    Shot shot = shotArr.get(j);
    shot.show();
  }
}

void newAsteroid() {

  //We assign the Asteroid a random x position based on zero and full window width 
  float asteroidXPos= random(0, width);

  //We assign the Asteroid a random size based on our min and max values
  float asteroidSize= random(asteroidMinSize, asteroidMaxSize);

  // We add a new asteroid to the asteroid array
  asteroidArr.add(new Asteroid(asteroidXPos, asteroidSpeed, asteroidSize));
}

void createAsteroids() {

  translate(0, 0);
  // A for loop that loops through all asteroids
  for (int k=0; k<asteroidArr.size(); k++) {

    Asteroid asteroid = asteroidArr.get(k);
    asteroid.visualize();
  }
}

void checkHits() {

  //We loop through all shots we have created
  for (int l=0; l<shotArr.size(); l++) {

    //We want to compare each shot with all existing asteroids
    for (int m=0; m<asteroidArr.size(); m++) {

      //We declare a variable to access one shot object of the Class Shot at a time
      Shot shot = shotArr.get(l);

      //We call the functions in the Shot class to return variables for position and size
      shotX=shot.getXPos();
      shotY=shot.getYPos();
      shotSize=shot.getSize();

      //We declare a variable to access one asteroid object of the Class Asteroid at a time
      Asteroid asteroid = asteroidArr.get(m);

      //We call the functions in the Asteroid class to return variables for position and size
      asteroidX=asteroid.getXPos();
      asteroidY=asteroid.getYPos();
      asteroidSize=asteroid.getSize();

      //We check the boundaries using nestled if statements, just like in "Catch the Apple"
      if (asteroidY+asteroidSize>shotY&&asteroidY<shotY+shotSize) {
        if (asteroidX+asteroidSize>shotX&&asteroidX<shotX+shotSize) {

          //Once we know an asteroid has been hit, we set the function asteroidHit() to "true"
          asteroid.asteroidHit(true);

          //We remove the asteroid from the array
          asteroidArr.remove(m);
          points++; //We add points each time an asteroid is destroyed

        } else {
          asteroid.asteroidHit(false);
        }
      }
    }
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

  // If the asteroidTimer variable has reached the interval
  if (asteroidTimer>asteroidInterval) {
    newAsteroid(); // We add a new asteroid
    lastAsteroidCheck=timeSinceStart; // We save the current time since start
  } else {
    asteroidTimer=0; // The timer is reset
  }

  timeSinceStart=millis(); // Assign current time
  shotTimer=timeSinceStart-lastShotCheck; // Assign time since last shot
  asteroidTimer=timeSinceStart-lastAsteroidCheck; // Assign time since last asteroid
}

void checkDamage() {

  //We loop through all asteroids
  for (int n=0; n<asteroidArr.size(); n++) {

    //We declare a variable to access one asteroid object of the Class Asteroid at a time
    Asteroid asteroid = asteroidArr.get(n);

    //We call the functions in the Asteroid class to return variables for position and size
    asteroidX=asteroid.getXPos();
    asteroidY=asteroid.getYPos();
    asteroidSize=asteroid.getSize();
    
  // We check all asteroids against our player for collision
    if (asteroidY+asteroidSize>playerY&&asteroidY<playerY+playerHeight) {
      if (asteroidX+asteroidSize>playerX-playerWidth/2&&asteroidX<playerX+playerWidth/2) {
        
        //Decrease life on hit
        life-=1;
        playerHit=true;
      } else {
        playerHit=false;
      }
    }
  }
}

void createLifeBar() {
  rectMode(CORNER);
  
  //Green lifebar
  fill(24, 148, 154);
  rect(20, 40, life, 40);
}

void drawPoints() {
  fill(200); 
  textAlign(CENTER, TOP); // Align text
  text(points, width/2, 30); // Assign text content
}

void gameOver() {
  background(100);
  fill(40);

  textAlign(CENTER, BOTTOM); // Align text 
  text("GAME OVER", width/2, 200); // Assign text content

  textAlign(CENTER, TOP); // Align text

  text("your score was:", width/2, 320); // Assign text content
  text(points, width/2, 400); // Assign text content
}