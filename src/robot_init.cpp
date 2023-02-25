#include "main.h"
#include "robot.h"

pros::Controller controller = pros::Controller(pros::E_CONTROLLER_MASTER);

pros::Motor RightFront = pros::Motor(RIGHT_FRONT_PORT);
pros::Motor RightBack = pros::Motor(RIGHT_BACK_PORT);

pros::Motor LeftFront = pros::Motor(LEFT_FRONT_PORT, true);
pros::Motor LeftBack = pros::Motor(LEFT_BACK_PORT, true);

pros::Motor Roller = pros::Motor(ROLLER_PORT, true);

pros::Motor IntakeOne = pros::Motor(INTAKE_ONE_PORT);
pros::Motor IntakeTwo = pros::Motor(INTAKE_TWO_PORT);

pros::Motor FlyWheel = pros::Motor(FLYWHEEL_PORT, pros::E_MOTOR_GEAR_600, true); //600 rpm cuz that's the highest and we don't have a gearbox

//sensors
pros::Imu gyro = pros::Imu(INERTIAL1_PORT);
pros::Imu gyro2 = pros::Imu(INERTIAL2_PORT);

pros::Optical optical = pros::Optical(OPTICAL_PORT);
pros::Optical speedIndicator = pros::Optical(INDICATOR_OPTICAL_PORT);

pros::ADIDigitalOut Strings = pros::ADIDigitalOut(STRINGS_PORT);
pros::ADIAnalogIn lineTracker = pros::ADIAnalogIn(LINE_TRACKER_PORT);
pros::ADIDigitalOut Adjuster = pros::ADIDigitalOut(ADJUSTER_PORT);
