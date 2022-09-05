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
int robot_x;
int robot_y;

int lastEncoderPositionX;
int lastEncoderPositionY;

//milliseconds between each position update
int PositionUpdateRate; //should NEVER be zero. Thread will take up entire processor usage and stop the code from running

bool stopThreads;

//functions used by the main file
void init(); //set variable values and stuff
void reset(); //resets rotation, odometers, variables, etc
void update_pos(); //update position based off of how fast the wheels of the robot are turning (will run in separate thread)
Points getPositionXY(); //returns the robots current position

//movement functions
