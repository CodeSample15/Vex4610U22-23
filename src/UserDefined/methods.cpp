/*
  * Author: Luke Crimi
  * Created: 2022

  * Contains all of the necessary methods and variables to easily 
    create autons and prep the initialize the robot before a game
*/

#include "UserDefined/methods.h"
#include "Screen/firstPage.hpp"
#include "api.h"
#include "robot.h"
#include <cmath>
#include <iostream>

AutonManager a_manager = AutonManager();

//initializing the pid objects with their respective tunes
PID turnPid = PID(1.3, 0, 0.1, 20, 25, 4);
PID movePid = PID(0.2, 0.03, 0.3, 20, 30, 6);
PID flyWheelPid = PID(0.43, 0.01, 0.15, 5, 300, 0, 127);

char TEAM_COLOR = 'r';

bool stopThreads;
bool autonStarted;

int flyWheelSpeed;

bool RunningSkills = false;

//for multithreading methods
bool processFinished = false; //for multithreading methods
PID* pidTemp;
PID* turnPIDTemp;
int amountTemp;
double speedTemp;

void set_pos(); //definition at the bottom of this file (sets starting position of the robot based off of what the user enters into the GUI)

void flyWheelThread()
{
  //to continuously update the flywheel speed using a custom pid
  double result;

  while(true) 
  {
    result = flyWheelPid.calculate(FlyWheel.get_actual_velocity(), flyWheelSpeed);
    result = (result > 0 ? result : 0); //clamp the result to be only positive
    result = (result <= 127 ? result : 127); //clamp the result to be within the correct range

    FlyWheel.move(result);
  }
}

//pros::Task f_thread(flyWheelThread);

void init()
{
  autonStarted = false;
  flyWheelSpeed = 0;

  Strings.set_value(0); //for the string launcher

  stopThreads = true;

  RightFront.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  RightBack.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  LeftFront.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  LeftBack.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

  FlyWheel.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

  //calibrate imu
  gyro.reset();
  gyro2.reset();

  while(gyro.is_calibrating() || gyro2.is_calibrating())
    pros::delay(50);
  gyro.tare();
  gyro2.tare();

  if(start_pos == FOUR || start_pos == FIVE) {
    gyro.set_rotation(-90);
  }
}

void reset_position() {
  RightFront.tare_position();
  RightBack.tare_position();
  LeftFront.tare_position();
  LeftBack.tare_position();
}

double get_avr_pos() {
  double one = RightFront.get_position();
  double two = RightBack.get_position();
  double three = LeftFront.get_position();
  double four = LeftBack.get_position();

  return (one+two+three+four) / 4;
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

  //map number between -180 and 180
  if(curRotation > 180)
    curRotation -= 360;
  else if(curRotation < -180)
    curRotation += 360;

  return curRotation;
}


void set_drive_raw(int left, int right) 
{
  RightBack.move(right);
  RightFront.move(right);
  LeftBack.move(left);
  LeftBack.move(left);
}

void stop_drive()
{
  RightBack.brake();
  RightFront.brake();
  LeftBack.brake();
  LeftFront.brake();
}

//movement stuff (all of the drivetrain stuff will run with motor.move)
void hardDriveStop() 
{
  std::cout << "Breaking..." << std::endl;

  //may look pretty simple and unnecessary, but it plays a huge role of giving the bot enough time to fully slow down before continuing. Otherwise the bot may overshoot from not getting rid of all its momentum
  RightFront.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	RightBack.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	LeftFront.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	LeftBack.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

  std::cout << RightFront.get_actual_velocity() << std::endl;
  while(std::abs(RightFront.get_actual_velocity()) > 1) {
    RightFront.brake();
    RightBack.brake();
    LeftFront.brake();
    LeftBack.brake();

    pros::delay(10);
  }

  RightFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	RightBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	LeftFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	LeftBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

  std::cout << "Done breaking" << std::endl;
}


void Move(int amount, int speed, bool hardstop) 
{
  reset_position();

  if(amount > 0)
    speed *= -1;

  while(std::abs(get_avr_pos()) < amount) {
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


void Move(PID& pid, int amount, double s) 
{
  reset_position();
  double speed = 0;

  do {
    speed = pid.calculate(get_avr_pos(), amount);
    speed *= s;

    speed += (speed>0 ? 3 : -3); //add a minimum of 3 to the total voltage to prevent bot from getting stuck

    RightFront.move(speed);
    RightBack.move(speed);
    LeftFront.move(speed);
    LeftBack.move(speed);
  } while(std::abs(pid.error) > 15);

  hardDriveStop();
}


void Move(PID& pid, PID& turnPID, int amount, double s) 
{
  reset_position();
  double startRot = gyro.get_rotation(); //so that the current rotation of the gyro doesn't get messed up. This is for position tracking purposes and should be remove if we get a second gyro

  double turnAmount = 0;
  double speed;

  do {
    speed = pid.calculate(get_avr_pos(), amount) * s;
    turnAmount = turnPID.calculate(gyro.get_rotation(), startRot);

    RightFront.move(speed - turnAmount);
    RightBack.move(speed - turnAmount);
    LeftFront.move(speed + turnAmount);
    LeftBack.move(speed + turnAmount);

    pros::delay(5);
  } while((std::abs(pid.error) > 15 || std::abs(turnPID.error) > 10) && !stopThreads);

  hardDriveStop();
}

void MoveWithTempValues()
{
  processFinished = false;
  Move(*pidTemp, *turnPIDTemp, amountTemp, speedTemp);
  processFinished = true;
}

void Move(PID& pid, PID& turnPID, int amount, double speed, float killTime) 
{
  pidTemp = &pid;
  turnPIDTemp = &turnPID;
  amountTemp = amount;
  speedTemp = speed;

  stopThreads = false;
  pros::Task driveThread(MoveWithTempValues);

  float elapsed = 0;

  while(elapsed < killTime) {
    pros::delay(20);
    elapsed += 0.02;

    if(processFinished)
      break;
  }

  if(elapsed >= killTime) {
    stopThreads = true; //kill the thread (?)
    pros::delay(100);
  }
}

void Turn(PID& turnPid, int amount, double speed) 
{
  gyro2.tare_rotation();
  int turnSpeed = 0;

  do {
    turnSpeed = turnPid.calculate(gyro2.get_rotation(), amount) * speed;

    RightFront.move(-turnSpeed);
    RightBack.move(-turnSpeed);
    LeftFront.move(turnSpeed);
    LeftBack.move(turnSpeed);
  } while(std::abs(turnPid.error) > 5);

  hardDriveStop();
}

void Turn(PID& turnPid, int amount, double speed, bool (*active)()) 
{
  gyro2.tare_rotation();
  int turnSpeed = 0;

  do {
    turnSpeed = turnPid.calculate(gyro2.get_rotation(), amount) * speed;

    RightFront.move(-turnSpeed);
    RightBack.move(-turnSpeed);
    LeftFront.move(turnSpeed);
    LeftBack.move(turnSpeed);
  } while(std::abs(turnPid.error) > 5 && active());

  hardDriveStop();
}

void TurnTo(PID& turnPid, int rotation, double speed) 
{
  double rot = rotation - getRegularRotation();

  Turn(turnPid, rot, speed);
}

void spinPrep()
{
  flyWheelSpeed = 300;
}

void spinUp()
{
  FlyWheel.move_velocity(400);
  flyWheelSpeed = 400;
}

void spinDown()
{
  flyWheelSpeed = 0;
  FlyWheel.brake();
}

bool flyRecovering()
{
  return std::abs(FlyWheel.get_actual_velocity() - flyWheelSpeed) > 5; //return whether the flywheel is within 10 rpm of the desired amount
}

void shoot()
{
  //for auton, wait until the flywheel has recovered before continuing
  while(flyRecovering())
    pros::delay(10);

  IntakeOne.tare_position();
  IntakeOne.move_relative(-100, 100);
    
  while(flyRecovering())
    pros::delay(10);
}

void spinRollerToColor(char col) {
  bool isAtColor = false;

  do {
    Roller.move_velocity(50);
    if(col == 'r')
      isAtColor = optical.get_rgb().red > (optical.get_rgb().blue + optical.get_rgb().green);
    else
      isAtColor = optical.get_rgb().blue > (optical.get_rgb().red + optical.get_rgb().green);;

    pros::delay(10);
  } while(!isAtColor);
}