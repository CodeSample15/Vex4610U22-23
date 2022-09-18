#include "api.h"

#define RIGHT_FRONT_PORT 9
#define RIGHT_BACK_PORT 14
#define LEFT_FRONT_PORT 8
#define LEFT_BACK_PORT 12

#define GYRO_PORT 3

#define X_ENCODER_PORT 19
#define Y_ENCODER_PORT 20


extern pros::Controller controller;

extern pros::Motor RightFront;
extern pros::Motor RightBack;

extern pros::Motor LeftFront;
extern pros::Motor LeftBack;

extern pros::Imu gyro;

extern pros::Rotation xEncoder;
extern pros::Rotation yEncoder;
