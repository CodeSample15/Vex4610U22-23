#include "api.h"
#include "robot.h"

pros::Controller controller = pros::Controller(pros::E_CONTROLLER_MASTER);

pros::Motor RightFront = pros::Motor(RIGHT_FRONT_PORT);
pros::Motor RightBack = pros::Motor(RIGHT_BACK_PORT);

pros::Motor LeftFront = pros::Motor(LEFT_FRONT_PORT, true);
pros::Motor LeftBack = pros::Motor(LEFT_BACK_PORT, true);

//sensors
pros::Imu gyro = pros::Imu(INERTIAL1_PORT);
pros::Imu gyro2 = pros::Imu(INERTIAL2_PORT);

pros::Rotation xEncoder = pros::Rotation(X_ENCODER_PORT);
pros::Rotation yEncoder = pros::Rotation(Y_ENCODER_PORT);
