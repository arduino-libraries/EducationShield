/*
*  _5.7_classes_demo
* 
* In this example, you will be introduced to how classes and objects work in actual code. 
* Note that the example has two tabs for the animal classes and one tab where we create our 
* objects and call the classes’ corresponding functions. 
* The tab where the action happens is what we will here on refer to as the main program.
*
* (c) 2013-2016 Arduino LLC.
*/

Bird myBird;
Rabbit myRabbit;

void setup() {
  myBird = new Bird(true);
  myRabbit = new Rabbit(false);
}

void draw() {
  myBird.flapWings();
  myRabbit.eatCarrot();
}