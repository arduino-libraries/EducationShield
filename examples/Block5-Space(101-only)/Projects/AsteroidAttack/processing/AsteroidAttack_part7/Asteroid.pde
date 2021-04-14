
class Asteroid {

  float xpos; // Asteroid x position
  float ypos; // Asteroid y position
  float asteroidSize=50; //The size of the asteroids
  float speed; // Speed of movement
  boolean isShot; // Determines if it has been hit

  // The Constructor is defined with arguments
  Asteroid(float Xpos, float inSpeed, float size) { 
    // Assign the incoming values to our class variables
    xpos = Xpos;
    asteroidSize = size;
    ypos=0-size;
    speed=inSpeed;
    isShot=false;
  }

  void visualize() {
    pushMatrix();

    translate(xpos, ypos); // We move the asteroid down every frame

    //Check whether asteroid has been shot
    if (!isShot) {

      if (asteroidSize<50) {
        image(img[1], 0, 0, asteroidSize, asteroidSize);  // Small asteroid image
        fill(100);
      } else if (asteroidSize>=50) {
        image(img[2], 0, 0, asteroidSize, asteroidSize);  // Large asteroid image
      }
    }
    ypos=ypos+speed; // Update the y position

    popMatrix();
  }

  void asteroidHit(boolean hit) {
    isShot=hit; // Returns true or false
  }

  float getXPos() {
    return xpos; // Returns the x position
  }

  float getYPos() {
    return ypos; // Returns the y position
  }

  float getSize() {
    return asteroidSize; // Returns the size
  }
}