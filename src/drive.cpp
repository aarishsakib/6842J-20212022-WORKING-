#include "main.h"

//18:1 -> Green Cartridge
//36:1 -> Red Cartridge
//6:1 ->Blue Cartridge

// Define Inertial Sensor
pros::Imu inertial(8);

//define Motors [FOR ALL OF THESE, CHANGE THE PORT, I AM NOT SURE ABOUT GEARSET, AND SOME MIGHT HAVE TO BE REVERSED]
pros::Motor rightFront(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);//defines front motor on the right side of the drive base
pros::Motor rightBack(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);//defines back motor on the right side of the drive base
pros::Motor leftFront(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);//defines front motor on the left side of the drive base
pros::Motor leftBack(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);// defines back motor on the left side of the drive base
pros::Motor leftArm(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);//defines left motor on the arm
pros::Motor rightArm(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);// defines right motor on the leftArm
pros::Motor intake(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);//
pros::Motor backClaw(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);//
pros::pneumatic pneumatic1(1, pros::);

PIDController drivePID(0.1, 30)
//type of object name (parameters)

//Variable and Constants

int TICS_PER_REVOLUTION = 360;

double WHEEL_DIAMETER = 4;

double pi = 3.14159;

void wait(int time) {
  //measured in milliseconds
  pros::delay(time);
}

void left(int speed){

  leftFront.move(speed);
  leftBack.move(speed);
}

void right(int speed){

  rightFront.move(speed);
  rightBack.move(speed);

}

void timedDrive(int time, int speed){
  left(speed);
  right(speed);
  wait(time);
  left(0);
  right(0);
}

void brake() {
  rightFront.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  rightBack.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  leftFront.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  leftBack.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

void coast() {
  rightFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  rightBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  leftFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  leftBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

void moveForward(int inches) {
  rightFront.tare_position();
  double target = TICS_PER_REVOLUTION * (inches / (WHEEL_DIAMETER * pi));

  while(rightFront.get_position() < target) {
    right(127);
    left(127);
    wait(5); //wait 5 milliseconds

  }
  right(0);
  left(0);
    //define target -> encoder degrees
    //while(current position < target) {
    //move forward
  //}
  //stop
}

// turn right for certain amount of degrees
void turnRight(int degrees, int speed) {
  while(inertial.get_rotation() < degrees) {
    left(speed);
    right(speed * (-1));
  }
  right(0);
  left(0);
}

void turnRightPID(int degrees) {
  double kP = 8;// defines gain(proportional) adjust using trial and error

  int time = 0;

  int minSpeed = 30; // adjust according to trial and error of one's own robot

  while(time < 40) {
    double error = degrees - (inertial.get_rotation()); // redefines value of error

    int speed = error * kP; // determines speed using proportional formula

    if(speed <= minSpeed && speed >= 0) {
      speed = minSpeed;
    }

    else if(speed <= 0 && speed >= -minSpeed) {
      speed = -minSpeed;
    }
    left(speed);
    right(-speed);

    if(fabs(error) < 1) {
      time ++; // adds one to variable time
      wait(2); // waits two milliseconds
    }
  }
  right(0);
  left(0);
}

void turnLeft(int degrees, int speed) {
  while(inertial.get_rotation() < degrees) {
    right(speed);
    left(speed * (-1));
  }
  right(0);
  left(0);
}

void turnLeftPID(int degrees) {
  double kP = 8;// defines gain(proportional)

  int time = 0;

  int minSpeed = 30; // adjust according to trial and error of one's own robot

  while(time < 40) {
    double error = degrees - (inertial.get_rotation()); // redefines value of error

    int speed = error * kP; // determines speed using proportional formula

    if(speed <= minSpeed && speed >= 0) {
      speed = minSpeed;
    }

    else if(speed <= 0 && speed >= -minSpeed) {
      speed = -minSpeed;
    }
    left(-speed);
    right(speed);

    if(fabs(error) < 1) {
      time ++; // adds one to variable time
      wait(2); // waits two milliseconds
    }
  }
  right(0);
  left(0);
}

void moveForwardPID(int inches) {
  rightFront.tare_position();

  double kP = 8;// defines gain(proportional)

  int time = 0;

  int minSpeed = 30; // adjust according to trial and error of robot

  double target = TICS_PER_REVOLUTION * (inches / (WHEEL_DIAMETER * pi));
  while(time < 40) {
    double error = target - rightFront.get_position(); // redefines value of error

    int speed = error * kP; // determines speed using proportional formula

    if(speed <= minSpeed && speed >= 0) {
      speed = minSpeed;
    }

    else if(speed <= minSpeed && speed <= 0) {
      speed = -minSpeed;
    }
    left(speed);
    right(speed);

    if(fabs(error) < 1) {
      time ++; // adds one to variable time
      wait(2); // waits two milliseconds
    }
  }
  right(0);
  left(0);
}

// Driver Control Functions

void driveOP() { // change all button controls once robot is built and is being programmed

  if (master.get_digital(DIGITAL_R1)) {// raise the main arm
    leftArm.move(127);
    rightArm.move(127);
  }
  if (master.get_digital(DIGITAL_R2)) {// lower the main arm
    leftArm.move(-127);
    rightArm.move(-127);
  }
  if (master.get_digital(DIGITAL_L1)) {// raise the back claw
    backClaw.move(127);
  }
  if (master.get_digital(DIGITAL_L2)) {// lower the back claw
    backClaw.move(-127);
  }
  if (master.get_digital(DIGITAL_UP)) { // close pneumatic system

  }
  if(master.get_digital(DIGITAL_X)) { // if button x is pressed
    rightBack.move(127);
  }
  rightFront.move(master.get_analog(ANALOG_RIGHT_Y));
  rightBack.move(master.get_analog(ANALOG_RIGHT_Y));
  leftFront.move(master.get_analog(ANALOG_LEFT_Y));
  leftBack.move(master.get_analog(ANALOG_LEFT_Y));
}
