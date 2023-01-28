#include "api.h"

#define RIGHT_FRONT_PORT 9
#define RIGHT_BACK_PORT 10
#define LEFT_FRONT_PORT 5
#define LEFT_BACK_PORT 8

#define ROLLER_PORT 12

#define INTAKE_ONE_PORT 11

#define FLYWHEEL_PORT 7

#define INERTIAL1_PORT 18
#define INERTIAL2_PORT 19

#define OPTICAL_PORT 4
#define INDICATOR_OPTICAL_PORT 12

#define INDEXER_PORT 'A'
#define STRINGS_PORT 'B'


extern pros::Controller controller;

extern pros::Motor RightFront;
extern pros::Motor RightBack;

extern pros::Motor LeftFront;
extern pros::Motor LeftBack;

extern pros::Motor Roller;

extern pros::Motor IntakeOne;

extern pros::Motor FlyWheel;

extern pros::Imu gyro;
extern pros::Imu gyro2;

extern pros::Optical optical;
extern pros::Optical speedIndicator;

extern pros::ADIDigitalOut Indexer;
extern pros::ADIDigitalOut Strings;