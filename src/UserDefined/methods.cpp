#include "UserDefined/methods.h"
#include "api.h"
#include "robot.h"
#include <cmath>
#include <iostream>

Points target_pos = Points(0,0);

//initializing the pid objects with their respective tunes
PID turnPid = PID(0.55, 0, 0.40, 15, 20, 3); //tunes are recycled from last year's robot
PID movePid = PID(0.09, 0.00, 0.07, 15);

bool stopThreads;

double robot_x;
double robot_y;

int lastEncoderPositionX;
int lastEncoderPositionY;

double maxSpeed = 0;

void init()
{
  robot_x = 0;
  robot_y = 0;

  lastEncoderPositionX = 0;
  lastEncoderPositionY = 0;

  xEncoder.reset();
  yEncoder.reset();

  stopThreads = true;

  RightFront.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  RightBack.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  LeftFront.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  LeftBack.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

  //calibrate imu
  gyro.reset();

  while(gyro.is_calibrating())
    pros::delay(50);
  gyro.tare();

  pros::delay(1000); //give the IMU an extra second

  //starting position tracking thread once everything is initialized
  pros::Task t(update_pos);
}

void reset() //shouldn't really be a need for this, but I'm putting this here just in case
{
  //reset position
  robot_x = 0;
  robot_y = 0;
  lastEncoderPositionX = 0;
  lastEncoderPositionY = 0;
  xEncoder.reset();
  yEncoder.reset();
}

void update_pos() //this should ALWAYS be running to keep track of the robot's position
{
  //ensure that everything was reset
  robot_x = 0;
  robot_y = 0;
  lastEncoderPositionX = 0;
  lastEncoderPositionY = 0;

  xEncoder.set_position(0);
  yEncoder.set_position(0);

  while (true) {
    //add on to the amount moved between delay times (PositionUpdateRate)
    int xpos = xEncoder.get_position();
    int ypos = yEncoder.get_position();

    double xDist = lastEncoderPositionX - xpos;
    double yDist = lastEncoderPositionY - ypos;

    xDist /= 100; //because the values that come out of this are too high
    yDist /= 100;

    lastEncoderPositionX = xpos;
    lastEncoderPositionY = ypos;

    //change x and y location of the robot based off of the rotation of the robot
    double rot = getRegularRotation();

    robot_x += (sin(rot*PI/180) * yDist) + (cos(rot*PI/180) * xDist);
    robot_y += (cos(rot*PI/180) * yDist) + (sin(rot*PI/180) * xDist);

    pros::delay(10); //change this value to update the frequency that the position of the robot is updated
  }
}

Points getPositionXY()
{
  return Points((double)robot_x, (double)robot_y);
}

double getRotation() 
{
  double curRotation = gyro.get_rotation();

  //mapping the rotation between -360 and 360
  int times = (int)(curRotation / 360);
  curRotation -= (360 * times);

  //mapping the next value between 0 and 360
  if(curRotation < 0)
    curRotation += 360;

  return curRotation;
}

double getRegularRotation()
{
  double curRotation = gyro.get_rotation();

  int times = (int)(curRotation / 360);
  curRotation -= (360 * times);

  if(curRotation > 180)
    curRotation -= 360;
  else if(curRotation < -180)
    curRotation += 360;

  return curRotation;
}


//movement stuff (all of the drivetrain stuff will run with motor.move)
void hardDriveStop() 
{
  //may look pretty simple and unnecessary, but it plays a huge role of giving the bot enough time to fully slow down before continuing. Otherwise the bot may overshoot from not getting rid of all its momentum 
  RightFront.brake();
  RightBack.brake();
  LeftFront.brake();
  LeftBack.brake();

  while(abs(RightFront.get_actual_velocity()) > 1)
    pros::delay(10);
}


void Move(int amount, int speed, bool hardstop) 
{
  yEncoder.reset();

  if(amount > 0)
    speed *= -1;

  while(abs(yEncoder.get_position()) < amount) {
    RightFront.move(speed);
    RightBack.move(speed);
    LeftFront.move(speed);
    LeftBack.move(speed);

    pros::delay(10);
  }

  if(hardstop) {
    hardDriveStop();
  }
}


void Move(PID& pid, int amount, double speed) 
{
  yEncoder.reset();

  do {
    speed = pid.calculate(yEncoder.get_position(), amount) * speed;

    RightFront.move(speed);
    RightBack.move(speed);
    LeftFront.move(speed);
    LeftBack.move(speed);

    pros::delay(10);
  } while(abs((int)pid.error) > 2);

  hardDriveStop();
}


void Move(PID& pid, PID& turnPID, int amount, double speed) 
{
  yEncoder.reset();
  double startRot = gyro.get_rotation(); //so that the current rotation of the gyro doesn't get messed up. This is for position tracking purposes and should be remove if we get a second gyro

  double turnAmount = 0;

  do {
    speed = pid.calculate(yEncoder.get_position(), amount) * speed;
    turnAmount = turnPID.calculate(gyro.get_rotation(), startRot);

    RightFront.move(speed - turnAmount);
    RightBack.move(speed - turnAmount);
    LeftFront.move(speed + turnAmount);
    LeftBack.move(speed + turnAmount);

    pros::delay(10);
  } while(abs((int)pid.error) > 2 || abs((int)turnPID.error) > 3);

  hardDriveStop();
}


void Turn(PID& turnPid, int amount, double speed) 
{
  int startRot = gyro.get_rotation();

  do {
    speed = turnPid.calculate(gyro.get_rotation(), gyro.get_rotation() + (amount - startRot)) * speed;
    std::cout << speed;
    RightFront.move(-speed);
    RightBack.move(-speed);
    LeftFront.move(speed);
    LeftBack.move(speed);

    pros::delay(10);
  } while(abs((int)turnPid.error) > 3);

  hardDriveStop();
}

void Turn(PID& turnPid, int amount, double speed, bool (*active)()) 
{
  int startRot = gyro.get_rotation();

  do {
    speed = turnPid.calculate(gyro.get_rotation(), gyro.get_rotation() + (amount - startRot)) * speed;

    RightFront.move(-speed);
    RightBack.move(-speed);
    LeftFront.move(speed);
    LeftBack.move(speed);

    pros::delay(10);
  } while(abs((int)turnPid.error) > 3 && active());

  hardDriveStop();
}

void TurnToRotation(PID& turnPid, int degree, double speed) 
{
  //get local rotation
  double curRot = getRotation();

  //determine quickest way to turn to that rotation
  double distOne = degree - curRot;
  double distTwo = (degree + 360) - curRot;
  double distThree = (curRot + 360) - degree;

  //excecute turn
  if(abs(distOne) < abs(distTwo) && abs(distOne) < abs(distThree)) {
    std::cout << "Turn one was faster" << std::endl;
    Turn(turnPid, degree - curRot, speed);
  }
  else if(abs(distTwo) < abs(distThree)) {
    std::cout << "Turn two was faster" << std::endl;
    Turn(turnPid, (degree + 360) - curRot, speed);
  }
  else {
    std::cout << "Turn three was faster" << std::endl;
    Turn(turnPid, (curRot + 360) - degree, speed);
  }
}

void TurnToRotation(PID& turnPid, int degree, double speed, bool (*active)()) 
{
  //get local rotation
  double curRot = getRotation();

  //determine quickest way to turn to that rotation
  double distOne = degree - curRot;
  double distTwo = (degree + 360) - curRot;
  double distThree = (curRot + 360) - degree;

  //excecute turn
  if(abs(distOne) < abs(distTwo) && abs(distOne) < abs(distThree)) {
    std::cout << "Turn one was faster" << std::endl;
    Turn(turnPid, degree - curRot, speed, active);
  }
  else if(abs(distTwo) < abs(distThree)) {
    std::cout << "Turn two was faster" << std::endl;
    Turn(turnPid, (degree + 360) - curRot, speed, active);
  }
  else {
    std::cout << "Turn three was faster" << std::endl;
    Turn(turnPid, (curRot + 360) - degree, speed, active);
  }
}