#include "api.h"

#define RIGHT_FRONT_PORT 1
#define RIGHT_BACK_PORT 2
#define LEFT_FRONT_PORT 3
#define LEFT_BACK_PORT 4

#define GYRO_PORT 5

#define Y_ENCODER_PORT 6


extern pros::Controller controller;

extern pros::Motor RightFront;
extern pros::Motor RightBack;

extern pros::Motor LeftFront;
extern pros::Motor LeftBack;

extern pros::Imu gyro;

extern pros::Rotation yEncoder;
