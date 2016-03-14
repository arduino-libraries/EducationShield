
class Star {

  float xpos; // Star x position
  float ypos; // Star y position
  float starSize; // Star size

  // The Constructor is defined with arguments
  Star(float xPos, float yPos, float size) { 
    // Assign the incoming values to our class variables
    xpos = xPos; 
    ypos = yPos; 
    starSize = size;
  }

  void display() {
    pushMatrix();
    
    noStroke();
    fill(255); // Make the stars white

    ellipseMode(CENTER); // Makes sure the ellipse is center position on pivot

    /*Make sure the universe is infinite by checking whenever a
     star is outside of the window*/
    if (xpos>width) { // If the x position is larger than screen width
      xpos=0;
    } else if (xpos<0) { // If the x position is less than 0
      xpos=width;
    }
    ellipse(xpos, ypos, starSize, starSize); // Draw the star
    
    popMatrix();
  }

  void move(float inVal) {
    //Divide the inVal to create a parallax effect
    xpos -= inVal/10;
  }
}