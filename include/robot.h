#include "api.h"

#define RIGHT_FRONT_PORT 1
#define RIGHT_BACK_PORT 5
#define LEFT_FRONT_PORT 8
#define LEFT_BACK_PORT 9

#define GYRO_PORT 7

#define X_ENCODER_PORT 2
#define Y_ENCODER_PORT 10


extern pros::Controller controller;

extern pros::Motor RightFront;
extern pros::Motor RightBack;

extern pros::Motor LeftFront;
extern pros::Motor LeftBack;

extern pros::Imu gyro;

extern pros::Rotation xEncoder;
extern pros::Rotation yEncoder;
