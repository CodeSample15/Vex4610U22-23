#include "api.h"

#define RIGHT_FRONT_PORT 16
#define RIGHT_BACK_PORT 17
#define LEFT_FRONT_PORT 15
#define LEFT_BACK_PORT 11

#define FLYWHEEL_PORT 2

#define INERTIAL1_PORT 7
#define INERTIAL2_PORT 8

#define X_ENCODER_PORT 1
#define Y_ENCODER_PORT 3


extern pros::Controller controller;

extern pros::Motor RightFront;
extern pros::Motor RightBack;

extern pros::Motor LeftFront;
extern pros::Motor LeftBack;

extern pros::Motor FlyWheel;

extern pros::Imu gyro;
extern pros::Imu gyro2;

extern pros::Rotation xEncoder;
extern pros::Rotation yEncoder;
