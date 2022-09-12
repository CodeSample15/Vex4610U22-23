#include "UserDefined/methods.h"
#include "api.h"
#include "robot.h"
#include <cmath>

//initializing the pid objects with their respective tunes
PID turnPid(0.55, 0.005, 0.40, 15, 20, 3); //tunes are recycled from last year's robot
PID movePid(0.09, 0.00, 0.07, 15);

bool stopThreads;

int robot_x;
int robot_y;

int lastEncoderPositionY;


void init()
{
  robot_x = 0;
  robot_y = 0;

  lastEncoderPositionY = 0;

  stopThreads = true;
  PositionUpdateRate = 50;

  RightFront.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  RightBack.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  LeftFront.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  LeftBack.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

  //calibrate imu
  while(gyro.is_calibrating())
    pros::delay(10);
}

void reset() //shouldn't really be a need for this, but I'm putting this here just in case
{
  //reset position 
  robot_x = 0;
  robot_y = 0;
  lastEncoderPositionY = 0;
  yEncoder.reset();

  //reset IMU
  gyro.reset();
}

void update_pos() //this should ALWAYS be running to keep track of the robot's position
{
  while (true) {
    //add on to the amount moved between delay times (PositionUpdateRate)
    int yDist = yEncoder.get_value() - lastEncoderPositionY;

    lastEncoderPositionY = yEncoder.get_value();

    //change x and y location of the robot based off of the rotation of the robot
    double rot = gyro.get_rotation();

    robot_x += sin(rot*PI/180) * yDist;
    robot_y += cos(rot*PI/180) * yDist;

    pros::delay(PositionUpdateRate);
  }
}

Points getPositionXY()
{
  return Points((double)robot_x, (double)robot_y);
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

  while(abs(yEncoder.get_value()) < amount) {
    RightFront.move(speed);
    RightBack.move(speed);
    LeftFront.move(speed);
    LeftBack.move(speed);
  }

  if(hardstop) {
    hardDriveStop();
  }
}


void Move(PID& pid, int amount, double speed) 
{
  yEncoder.reset();

  do {
    speed = pid.calculate(yEncoder.get_value(), amount) * speed;

    RightFront.move(speed);
    RightBack.move(speed);
    LeftFront.move(speed);
    LeftBack.move(speed);
  } while(abs((int)pid.error) > 2);

  hardDriveStop();
}


void Move(PID& pid, PID& turnPID, int amount, double speed) 
{
  yEncoder.reset();
  double startRot = gyro.get_rotation(); //so that the current rotation of the gyro doesn't get messed up. This is for position tracking purposes and should be remove if we get a second gyro

  double turnAmount = 0;

  do {
    speed = pid.calculate(yEncoder.get_value(), amount) * speed;
    turnAmount = turnPID.calculate(gyro.get_rotation(), startRot);

    RightFront.move(speed - turnAmount);
    RightBack.move(speed - turnAmount);
    LeftFront.move(speed + turnAmount);
    LeftBack.move(speed + turnAmount);
  } while(abs((int)pid.error) > 2 || abs((int)turnPID.error) > 3);

  hardDriveStop();
}


void Turn(PID& turnPid, int amount, double speed) 
{
  int startRot = gyro.get_rotation();

  do {
    speed = turnPid.calculate(gyro.get_rotation(), amount - startRot) * speed;

    RightFront.move(-speed);
    RightBack.move(-speed);
    LeftFront.move(speed);
    LeftBack.move(speed);
  } while(abs((int)turnPid.error) > 3);

  hardDriveStop();
}