#include "UserDefined/methods.h"
#include "Screen/firstPage.hpp"
#include "api.h"
#include "robot.h"
#include <cmath>
#include <iostream>

AutonManager a_manager = AutonManager();

Points target_pos = Points(-1480, -4645);

//initializing the pid objects with their respective tunes
PID turnPid = PID(1.5, 0.001, 0.15, 40, 20, 10);
PID movePid = PID(0.09, 0.00, 0.07, 15);

bool stopThreads;

double robot_x;
double robot_y;

int lastEncoderPositionX;
int lastEncoderPositionY;
int lastRotationValue; //last value to be stored in the imu

double xEncoderOffset;

bool autonStarted;
bool runningAuton;

int flyWheelSpeed;
int maxShootDistance; //how far the bot has to be from the goal before it can't score with max rpm

void set_pos(); //definition at the bottom of this file (sets starting position of the robot based off of what the user enters into the GUI)

void init()
{
  indexerBack();

  autonStarted = false;
  runningAuton = false;

  lastEncoderPositionX = 0;
  lastEncoderPositionY = 0;
  lastRotationValue = 0;

  xEncoderOffset = 1700;

  flyWheelSpeed = 0;

  xEncoder.reset();
  yEncoder.reset();

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

  //set start position of the robot based off of what the user selected during auton selection
  set_pos();

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
  lastRotationValue = 0;
  xEncoder.reset();
  yEncoder.reset();

  indexerBack();
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
    double curRotation = gyro.get_rotation();
    double changeInRot = curRotation - lastRotationValue;
    if(abs(changeInRot) < 0.8)
      changeInRot = 0; //deadzone for IMU change

    double xDist = lastEncoderPositionX - xpos;
    double yDist = lastEncoderPositionY - ypos;
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
    robot_y += (cos(theta) * yDist) + (sin(theta) * xDist);

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
  while(abs(RightFront.get_actual_velocity()) > 1) {
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
  } while(abs((int)turnPid.error) > 5);

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
  } while(abs((int)turnPid.error) > 5 && active());

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

void spinPrep()
{
  FlyWheel.move_velocity(300); //spin up at 50% max RPM (600rpm is the max). Motor thinks it's going to 300rpm, but it's actually going higher due to the lack of a gearbox
}

void spinUp()
{
  double dist = getPositionXY().distanceTo(target_pos) - 100; //TUNE THIS (Min distance)

  int speed = ((dist/maxShootDistance)*100) + 500; //rpm should increase as distance increases TUNE THIS

  FlyWheel.move_velocity(speed);
  flyWheelSpeed = speed;
}

void spinDown()
{
  FlyWheel.brake(); //coasting brake mode
  flyWheelSpeed = 0;
}

bool flyRecovering()
{
  return (abs(FlyWheel.get_actual_velocity()) < abs(flyWheelSpeed)-10);
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
    if(flyRecovering) {
      controller.rumble("--");
    }
    else {
      indexerForward();
    }

  }
  else {

    //for auton, wait until the flywheel has recovered before continuing
    while(flyRecovering)
      pros::delay(10);

    indexerForward();
    
    while(flyRecovering)
      pros::delay(10);

    indexerBack();

  }
}


void set_pos() 
{
  switch(start_pos)
  {
    case ONE:
      robot_x = 0;
      robot_y = 0;
      gyro.set_rotation(0);
      break;

    case TWO:
      robot_x = 0;
      robot_y = 0;
      gyro.set_rotation(0);
      break;

    case THREE:
      robot_x = 0;
      robot_y = 0;
      gyro.set_rotation(0);
      break;

    case FOUR:
      robot_x = 0;
      robot_y = 0;
      gyro.set_rotation(0);
      break;

    case FIVE:
      robot_x = 0;
      robot_y = 0;
      gyro.set_rotation(0);
      break;
  }
}