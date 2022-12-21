#include "api.h"

#define RIGHT_FRONT_PORT 10
#define RIGHT_BACK_PORT 13
#define LEFT_FRONT_PORT 4
#define LEFT_BACK_PORT 5

#define ROLLER_PORT 11

#define INTAKE_ONE_PORT 3
#define INTAKE_TWO_PORT 19

#define FLYWHEEL_PORT 8

#define INERTIAL1_PORT 14
#define INERTIAL2_PORT 15

#define OPTICAL_PORT 7

#define X_ENCODER_PORT 16
#define Y_ENCODER_PORT 17

#define INDEXER_PORT 'A'
#define STRINGS_PORT 'H'


extern pros::Controller controller;

extern pros::Motor RightFront;
extern pros::Motor RightBack;

extern pros::Motor LeftFront;
extern pros::Motor LeftBack;

extern pros::Motor Roller;

extern pros::Motor IntakeOne;
extern pros::Motor IntakeTwo;

extern pros::Motor FlyWheel;

extern pros::Imu gyro;
extern pros::Imu gyro2;

extern pros::Optical optical;

extern pros::Rotation xEncoder;
extern pros::Rotation yEncoder;

extern pros::ADIDigitalOut Indexer;
extern pros::ADIDigitalOut Strings;
