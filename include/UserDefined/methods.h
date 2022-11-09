/*
  * Author: Luke Crimi
  * Date created: 9/4/2022
  * Project: PositionTracking
  * This holds all of the functions and variables used by the main script
*/

#pragma once
#define PI 3.1415926

#include "points.h"
#include "pid.h"
#include "AutonManager.h"

//auton manager
extern AutonManager a_manager;

//location of the goal (TBD)
extern Points target_pos;

//PID objects
extern PID turnPid;
extern PID movePid;

//variables
extern double robot_x;
extern double robot_y;

extern int lastEncoderPositionX;
extern int lastEncoderPositionY;

extern double xEncoderOffset;

extern bool stopThreads;
extern bool autonStarted;
extern bool runningAuton;

//functions used by the main file
void init(); //set variable values and stuff
void reset(); //resets rotation, odometers, variables, etc
void update_pos(); //update position based off of how fast the wheels of the robot are turning (will run in separate thread)
Points getPositionXY(); //returns the robots current position
double getRotation(); //returns the local rotation of the robot
double getRegularRotation(); //between -360 and 360

//movement functions
void hardDriveStop();
void Move(int amount, int speed, bool hardstop);
void Move(PID& pid, int amount, double speed);
void Move(PID& pid, PID& turnPID, int amount, double speed); //move function, but with rotation locking as well
//void MoveUntilLine(int speed); (maybe)
void MoveTo(PID& pid, PID& turnPid, int x, int y, double turnSpeed, double moveSpeed); //move to XY position by turning first and then moving distance between the robot and the desired point

void Turn(PID& turnPid, int amount, double speed);
void Turn(PID& turnPid, int amount, double speed, bool (*active)()); //with a lambda to break out of the turn if a condition is met (for autonomous turning during driver control)

void TurnToRotation(PID& turnPid, int degree, double speed);
void TurnToRotation(PID& turnPid, int degree, double speed, bool (*active)()); //same thing as the turn overload (driver control autonomous turning)

void spinPrep(); //warm the motors by building momentum in the fly wheel (faster to get to full speed)
void spinUp(); //set the flywheel's speed according to the robot's position 
void spinDown(); //turn off the flywheel