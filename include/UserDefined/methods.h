/*
  * Author: Luke Crimi
  * Date created: 9/4/2022
  * This holds all of the functions and variables used by the main script
*/

#pragma once
#define PI 3.1415926

#include "pid.h"
#include "AutonManager.h"

//auton manager
extern AutonManager a_manager;

//PID objects
extern PID turnPid;
extern PID movePid;
extern PID flyWheelPid;

//constants
extern char TEAM_COLOR;

//variables
extern bool stopThreads;
extern bool autonStarted;

extern int flyWheelSpeed;

extern bool RunningSkills;

//functions used by the main file
void init(); //set variable values and stuff
double getRotation(); //returns the local rotation of the robot
double getRegularRotation(); //between -360 and 360

//movement functions
void reset_position();
double get_avr_pos(); //get the average position between the four drive motors

void set_drive_raw(int left, int right);
void stop_drive();

void hardDriveStop();
void Move(int amount, int speed, bool hardstop);
void Move(PID& pid, int amount, double speed);
void Move(PID& pid, PID& turnPID, int amount, double speed); //move function, but with rotation locking as well
void Move(PID& pid, PID& turnPID, int amount, double speed, float killTime);
//void MoveUntilLine(int speed); (maybe)

void Turn(PID& turnPid, int amount, double speed);
void Turn(PID& turnPid, int amount, double speed, bool (*active)()); //with a lambda to break out of the turn if a condition is met (for autonomous turning during driver control)
void TurnTo(PID& turnPid, int rotation, double speed);

void spinRollerToColor(char col);

void spinPrep(); //warm the motors by building momentum in the fly wheel (faster to get to full speed)
void spinUp(); //set the flywheel's speed according to the robot's position 
void spinDown(); //turn off the flywheel

bool flyRecovering(); //if the flywheel is still recovering from the last fire

void shoot(bool driving); //toggle the indexer and shoot a disk but only if the flywheel is up to speed. Boolean is for whether this is an auton or not