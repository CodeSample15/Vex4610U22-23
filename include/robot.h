#include "api.h"

#define RIGHT_FRONT_PORT 16
#define RIGHT_BACK_PORT 17
#define LEFT_FRONT_PORT 15
#define LEFT_BACK_PORT 11

#define INTAKE_ONE_PORT 4
#define INTAKE_TWO_PORT 5

#define FLYWHEEL_PORT 3

#define INERTIAL1_PORT 10
#define INERTIAL2_PORT 8

#define X_ENCODER_PORT 2
#define Y_ENCODER_PORT 1

#define INDEXER_PORT 9


extern pros::Controller controller;

extern pros::Motor RightFront;
extern pros::Motor RightBack;

extern pros::Motor LeftFront;
extern pros::Motor LeftBack;

extern pros::Motor IntakeOne;
extern pros::Motor IntakeTwo;

extern pros::Motor FlyWheel;

extern pros::Imu gyro;
extern pros::Imu gyro2;

extern pros::Rotation xEncoder;
extern pros::Rotation yEncoder;

extern pros::ADIDigitalOut Indexer;
