#include "api.h"

#define RIGHT_FRONT_PORT 1
#define RIGHT_BACK_PORT 7
#define LEFT_FRONT_PORT 16
#define LEFT_BACK_PORT 17

#define ROLLER_PORT 20

#define INTAKE_ONE_PORT 2

#define FLYWHEEL_PORT 3

#define INERTIAL1_PORT 4
#define INERTIAL2_PORT 8

#define OPTICAL_PORT 5
#define INDICATOR_OPTICAL_PORT 12

#define STRINGS_PORT 'A'


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

extern pros::ADIDigitalOut Strings;