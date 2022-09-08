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

int lastEncoderPositionX;
int lastEncoderPositionY;


void init()
{
  stopThreads = true;
  PositionUpdateRate = 50;
}

void reset()
{
  robot_x = 0;
  robot_y = 0;

  lastEncoderPositionX = 0;
  lastEncoderPositionY = 0;

  yEncoder.reset();

  //reset IMU
  gyro.reset();
  while(gyro.is_calibrating())
    pros::delay(10);
}

void update_pos()
{
  stopThreads = false; //threads start running, make sure they don't stop until a reset is called

  while (!stopThreads) {
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
