/*  Player
* You can play a sound from an SD card using the Player class.
* The sound file need to be 8bit, uncompressed WAV. 
* 
* Or use one of the .wav files in the BoomBox example. 
* 
* (c) 2013-2016 Arduino LLC.
*/

#include <EducationShield.h>

Player player=Player();

void setup(){
  //Initialize the sound player
  player.begin();
}

void loop(){
  //play(filename)
  //  filename: filename of the sound stored in SD card
  //		must be a .wav file
  //
  //Play the file named "0.wav" on SD card
  player.play("0.wav");
  delay(1000);
}
