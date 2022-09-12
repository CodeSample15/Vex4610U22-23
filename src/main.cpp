#include <string>
#include "UserDefined/methods.h"
#include "UserDefined/points.h"
#include "robot.h"
#include "main.h"

void display()
{
	//thread to display important debug information
	pros::lcd::initialize();

	while(true) {
		pros::lcd::set_text(1, "X Position: " + std::to_string(robot_x));
		pros::lcd::set_text(2, "Y Position: " + std::to_string(robot_y));
		pros::lcd::set_text(3, "Robot rotation: " + std::to_string(gyro.get_rotation()));
		pros::lcd::set_text(4, "Right front temp: " + std::to_string(RightFront.get_temperature()));
		pros::lcd::set_text(5, "Right back temp: " + std::to_string(RightBack.get_temperature()));
		pros::lcd::set_text(6, "Left front temp: " + std::to_string(LeftFront.get_temperature()));
		pros::lcd::set_text(7, "Left back temp: " + std::to_string(LeftBack.get_temperature()));

		pros::delay(20);

		pros::lcd::clear();
	}
}

void initialize()
{
	pros::Task t(display); //start the display task

	init(); //all the initialization will happen in the methods file 
}

void disabled() {}

void competition_initialize()
{
}

void autonomous()
{

}

void opcontrol() {
	//split drive (left is front/back, right is right/left)
	stopThreads = true; //stop all threads that were running during auton (REMEMBER TO RESTART IMPORTANT ONES)

	//for feeding information to the right and left motors from the controller input
	int rightSpeed;
	int leftSpeed;

	while (true) {
		rightSpeed = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) - controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		leftSpeed = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) + controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		RightFront.move(rightSpeed);
		RightBack.move(rightSpeed);
		LeftFront.move(leftSpeed);
		LeftBack.move(leftSpeed);

		pros::delay(20);
	}
}
