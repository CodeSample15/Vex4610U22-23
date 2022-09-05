#include "api.h"
#include "robot.h"

pros::Controller controller = pros::Controller(pros::E_CONTROLLER_MASTER);

pros::Motor RightMotor = pros::Motor(RIGHT_MOTOR_PORT);
pros::Motor LeftMotor = pros::Motor(LEFT_MOTOR_PORT);

pros::Imu gyro = pros::Imu(GYRO_PORT);

pros::ADIEncoder yEncoder = pros::ADIEncoder(Y_ENCODER_TOP_PORT, Y_ENCODER_BOTTOM_PORT, false);
