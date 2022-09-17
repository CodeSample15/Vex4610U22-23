#include "api.h"
#include "robot.h"

pros::Controller controller = pros::Controller(pros::E_CONTROLLER_MASTER);

pros::Motor RightFront = pros::Motor(RIGHT_FRONT_PORT, true);
pros::Motor RightBack = pros::Motor(RIGHT_BACK_PORT, true);

pros::Motor LeftFront = pros::Motor(LEFT_FRONT_PORT);
pros::Motor LeftBack = pros::Motor(LEFT_BACK_PORT);

//sensors
pros::Imu gyro = pros::Imu(GYRO_PORT);

pros::Rotation xEncoder = pros::Rotation(X_ENCODER_PORT);
pros::Rotation yEncoder = pros::Rotation(Y_ENCODER_PORT);
