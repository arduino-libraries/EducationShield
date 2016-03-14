
class Shot {

  float xpos; // Shot x position
  float ypos; // Shot y position
  float shotPos; // Variable to subtract the y axis speed of the shot
  float size=10; //The size of the shots

  // The Constructor is defined with arguments
  Shot(float xPos, float yPos) {
    // Assign the incoming values to our class variables
    xpos=xPos;
    ypos=yPos;
    shotPos=0; // The y position starts on 0
  }

  void show() {
    pushMatrix();
    
    shotPos-=8; // Subtract the position value

    fill(255); 
    rectMode(CENTER);

    translate(xpos, ypos+shotPos); // We move the shot up every frame

    ellipse(0, 0, size, size); // Draw the shot

    popMatrix();
  }

  float getXPos() {
    return xpos; // Returns the x position
  }

  float getYPos() {
    return ypos+shotPos; // Returns the y position
  }

  float getSize() {
    return size; // Returns the size
  }

  float getShotInterval() {
    return shotInterval; // Returns the shot interval
  }
}