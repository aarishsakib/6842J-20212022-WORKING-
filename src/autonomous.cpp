#include "main.h"

void autonomous() {

  moveForward(10);// move forward 10 inches
  turnRight(90, 127);//turn right 90 degrees at max speed
  moveForward(50);//move forward 50 inches
  turnLeft(135, 127);//turn left 135 degrees at max speed
  moveForward(20);//move forward 20 inches
}
