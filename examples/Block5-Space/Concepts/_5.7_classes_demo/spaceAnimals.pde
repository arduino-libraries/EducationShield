Bird myBird;
Rabbit myRabbit;

void setup() {
  myBird = new Bird(true);
  myRabbit = new Rabbit(false);
}

void draw() {
  myBird.fly();
  myRabbit.eatCarrot();
}