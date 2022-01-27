void moveForward(int inches);//move forward
void driveOP();
void wait(int time);// wait
void left(int speed);// left side of drive base moves
void right(int speed);// right side of the drive base moves
void timedDrive(int time, int speed);
void brake();
void coast();
void turnLeft(int degrees, int speed);// turn robot left at certain speed to a certain direction using inertial sensor
void turnRight(int degrees, int speed);// turn robot right at certain speed to a certain direction using inertial sensor
void turnRightPID(int degrees);// robot turns right using a PID controller
void turnLeftPID(int degrees);// robot turns left using a PID controller
void moveForwardPID(int inches);// robot moves forward using a PID controller
