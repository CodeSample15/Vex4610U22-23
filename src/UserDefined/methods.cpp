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

Points target_pos = Points(1200, 4137);

//initializing the pid objects with their respective tunes
PID turnPid = PID(1.5, 0.03, 0.25, 40, 20, 10);
PID movePid = PID(0.5, 0.00, 0.25, 20);

char TEAM_COLOR = 'r';

bool stopThreads;

double robot_x = 0;
double robot_y = 0;

int lastEncoderPositionX;
int lastEncoderPositionY;
int lastRotationValue; //last value to be stored in the imu

double xEncoderOffset;

bool autonStarted;

int flyWheelSpeed;
int maxShootDistance; //how far the bot has to be from the goal before it can't score with max rpm

bool RunningSkills = false;

void set_pos(); //definition at the bottom of this file (sets starting position of the robot based off of what the user enters into the GUI)

void init()
{
  indexerBack();

  autonStarted = false;

  lastEncoderPositionX = 0;
  lastEncoderPositionY = 0;
  lastRotationValue = 0;

  maxShootDistance = 10000;

  xEncoderOffset = 10000;

  flyWheelSpeed = 0;

  xEncoder.set_position(0);
  yEncoder.set_position(0);

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

  //starting position tracking thread once everything is initialized
  pros::Task t(update_pos);
}

void reset()
{
  //reset position
  lastEncoderPositionX = 0;
  lastEncoderPositionY = 0;
  lastRotationValue = 0;
  xEncoder.set_position(0);
  yEncoder.set_position(0);

  pros::delay(200); //give the position tracking thread some time to catch up

  robot_x = 0;
  robot_y = 0;

  indexerBack();
}

void update_pos() //this should ALWAYS be running to keep track of the robot's position
{
  //ensure that everything was reset
  lastEncoderPositionX = 0;
  lastEncoderPositionY = 0;

  xEncoder.set_position(0);
  yEncoder.set_position(0);

  set_pos();

  while (true) {
    //add on to the amount moved between delay times (PositionUpdateRate)
    int xpos = xEncoder.get_position();
    int ypos = yEncoder.get_position();
    double curRotation = gyro.get_rotation();
    double changeInRot = curRotation - lastRotationValue;
    if(std::abs(changeInRot) < 1)
      changeInRot = 0; //deadzone for IMU change

    double xDist = -(lastEncoderPositionX - xpos);
    double yDist = -(lastEncoderPositionY - ypos);
    xDist += (changeInRot * xEncoderOffset * PI) / 180; //account for the offset of the x encoder

    xDist /= 100; //because the values that come out of this are too high
    yDist /= 100;

    lastEncoderPositionX = xpos;
    lastEncoderPositionY = ypos;
    lastRotationValue = curRotation;

    //change x and y location of the robot based off of the rotation of the robot
    double rot = getRegularRotation();
    double theta = rot*PI/180;

    robot_x += (sin(theta) * yDist) + (cos(theta) * xDist);
    robot_y += (cos(theta) * yDist) + (sin(-theta) * xDist);

    pros::delay(20); //change this value to update the frequency that the position of the robot is updated
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

  //map number between -180 and 180
  if(curRotation > 180)
    curRotation -= 360;
  else if(curRotation < -180)
    curRotation += 360;

  return curRotation;
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
  yEncoder.reset();

  if(amount > 0)
    speed *= -1;

  while(std::abs(yEncoder.get_position()) < amount) {
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
  int start_pos = yEncoder.get_position();
  double speed = 0;

  do {
    speed = pid.calculate(yEncoder.get_position() - start_pos, -amount);
    speed *= -s;

    speed += (speed>0 ? 3 : -3); //add a minimum of 3 to the total voltage to prevent bot from getting stuck

    RightFront.move(speed);
    RightBack.move(speed);
    LeftFront.move(speed);
    LeftBack.move(speed);
  } while(std::abs(pid.error) > 2);

  hardDriveStop();
}


void Move(PID& pid, PID& turnPID, int amount, double s) 
{
  yEncoder.reset();
  double startRot = gyro.get_rotation(); //so that the current rotation of the gyro doesn't get messed up. This is for position tracking purposes and should be remove if we get a second gyro

  double turnAmount = 0;
  double speed;

  do {
    speed = pid.calculate(yEncoder.get_position(), amount) * s;
    turnAmount = turnPID.calculate(gyro.get_rotation(), startRot);

    RightFront.move(speed - turnAmount);
    RightBack.move(speed - turnAmount);
    LeftFront.move(speed + turnAmount);
    LeftBack.move(speed + turnAmount);

    pros::delay(10);
  } while(std::abs(pid.error) > 2 || std::abs(turnPID.error) > 3);

  hardDriveStop();
}

void MoveTo(PID& pid, PID& turnPid, int x, int y, double turnSpeed, double moveSpeed) {
  Points pos(robot_x, robot_y);
  Points target(x, y);
	int targetRot = pos.angleTo(target);
  int distance = pos.distanceTo(target);

  TurnToRotation(turnPid, targetRot, turnSpeed);
  Move(pid, distance, moveSpeed);
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

void TurnToRotation(PID& turnPid, int degree, double speed) 
{
  //get local rotation
  double curRot = getRotation();

  //determine quickest way to turn to that rotation
  double distOne = degree - curRot;
  double distTwo = (degree + 360) - curRot;
  double distThree = (curRot + 360) - degree;

  //excecute turn
  if(std::abs(distOne) < std::abs(distTwo) && std::abs(distOne) < std::abs(distThree)) {
    std::cout << "Turn one was faster" << std::endl;
    Turn(turnPid, degree - curRot, speed);
  }
  else if(std::abs(distTwo) < std::abs(distThree)) {
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
  if(std::abs(distOne) < std::abs(distTwo) && std::abs(distOne) < std::abs(distThree)) {
    std::cout << "Turn one was faster" << std::endl;
    Turn(turnPid, degree - curRot, speed, active);
  }
  else if(std::abs(distTwo) < std::abs(distThree)) {
    std::cout << "Turn two was faster" << std::endl;
    Turn(turnPid, (degree + 360) - curRot, speed, active);
  }
  else {
    std::cout << "Turn three was faster" << std::endl;
    Turn(turnPid, (curRot + 360) - degree, speed, active);
  }
}

void spinPrep()
{
  FlyWheel.move_velocity(300); //spin up at 50% max RPM (600rpm is the max). Motor thinks it's going to 300rpm, but it's actually going higher due to the lack of a gearbox
  flyWheelSpeed = 300;
}

void spinUp()
{
  double dist = getPositionXY().distanceTo(target_pos);

  double distPerc = (dist/maxShootDistance);
  distPerc *= distPerc;

  int speed = (distPerc*170) + 430; //rpm should increase as distance increases TUNE THIS

  FlyWheel.move_velocity(speed);
  flyWheelSpeed = (speed > 600 ? 600 : speed);
}

void spinDown()
{
  FlyWheel.brake(); //coasting brake mode
  flyWheelSpeed = 0;
}

bool flyRecovering()
{
  return std::abs(FlyWheel.get_actual_velocity() - flyWheelSpeed) > 5; //return whether the flywheel is within 10 rpm of the desired amount
}

void indexerBack()
{
  Indexer.set_value(false);
}

void indexerForward() 
{
  Indexer.set_value(true);
}


void shoot(bool driving)
{
  if(driving) {
    //for driving, warn the driver that the fly wheel isn't ready by rumbling
    indexerForward();
    pros::delay(300);
    indexerBack();
  }
  else {

    //for auton, wait until the flywheel has recovered before continuing
    while(flyRecovering())
      pros::delay(10);

    indexerForward();
    
    while(flyRecovering())
      pros::delay(10);

    indexerBack();

  }
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

void set_pos() 
{
  if(start_pos == ONE) {
    robot_x = 1110;
    robot_y = 0;
    gyro.set_rotation(0);
  }
  else if(start_pos == TWO) {
    robot_x = 2220;
    robot_y = 0;
    gyro.set_rotation(0);
  }
  else if(start_pos == THREE) {
    robot_x = 3215;
    robot_y = 0;
    gyro.set_rotation(0);
  }
  else if(start_pos == FOUR) {
    robot_x = 5590;
    robot_y = 1550;
    gyro.set_rotation(-90);
  }
  else if(start_pos == FIVE) {
    robot_x = 5590;
    robot_y = 2460;
    gyro.set_rotation(-90);
  }
}