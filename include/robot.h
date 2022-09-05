#include "api.h"

#define RIGHT_MOTOR_PORT 1
#define LEFT_MOTOR_PORT 2

#define GYRO_PORT 3

#define Y_ENCODER_TOP_PORT 'A'
#define Y_ENCODER_BOTTOM_PORT 'B'


extern pros::Controller controller;

extern pros::Motor RightMotor;
extern pros::Motor LeftMotor;

extern pros::Imu gyro;

extern pros::ADIEncoder yEncoder;
