/*
  * Author: Luke Crimi
  * Date created: 9/4/2022
  * Project: PositionTracking
  * This will hold all of the functions and variables used by the main script
*/

#pragma once
#define PI 3.1415926

#include "UserDefined/points.h"
#include "UserDefined/pid.h"

//PID objects
extern PID turnPid;
extern PID movePid;

//variables
extern int robot_x;
extern int robot_y;

extern int lastEncoderPositionY;

extern bool stopThreads;

//functions used by the main file
void init(); //set variable values and stuff
void reset(); //resets rotation, odometers, variables, etc
void update_pos(); //update position based off of how fast the wheels of the robot are turning (will run in separate thread)
Points getPositionXY(); //returns the robots current position

//movement functions
void hardDriveStop();
void Move(int amount, int speed, bool hardstop);
void Move(PID& pid, int amount, double speed);
void Move(PID& pid, PID& turnPID, int amount, double speed); //move function, but with rotation locking as well
//void MoveUntilLine(int speed); (maybe)

void Turn(PID& turnPid, int amount, double speed);