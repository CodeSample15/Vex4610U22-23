#include "api.h"

#define RIGHT_FRONT_PORT 11
#define RIGHT_BACK_PORT 5
#define LEFT_FRONT_PORT 2
#define LEFT_BACK_PORT 14

#define GYRO_PORT 6

#define X_ENCODER_PORT 4
#define Y_ENCODER_PORT 20


extern pros::Controller controller;

extern pros::Motor RightFront;
extern pros::Motor RightBack;

extern pros::Motor LeftFront;
extern pros::Motor LeftBack;

extern pros::Imu gyro;

extern pros::Rotation xEncoder;
extern pros::Rotation yEncoder;
