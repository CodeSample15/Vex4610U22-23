#include <string>
#include "UserDefined/methods.h"
#include "UserDefined/points.h"
#include "robot.h"
#include "main.h"

using namespace pros;

void display()
{
	//thread to display important debug information
	lcd::initialize();

	while(true) {
		lcd::set_text(1, "X Position: " + std::to_string(robot_x));
		lcd::set_text(2, "Y Position: " + std::to_string(robot_y));
		lcd::set_text(3, "Rotation: " + std::to_string(gyro.get_rotation()));
		lcd::set_text(4, "Right front temp: " + std::to_string(RightFront.get_temperature()));
		lcd::set_text(5, "Right back temp: " + std::to_string(RightBack.get_temperature()));
		lcd::set_text(6, "Left front temp: " + std::to_string(LeftFront.get_temperature()));
		lcd::set_text(7, "Left back temp: " + std::to_string(LeftBack.get_temperature()));

		delay(20);

		lcd::clear();
	}
}

void initialize()
{
	Task d(display); //start the display task
	Task t(update_pos); //start keeping track of the robot's position

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
		rightSpeed = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) - controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
		leftSpeed = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) + controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);

		RightFront.move(rightSpeed);
		RightBack.move(rightSpeed);
		LeftFront.move(leftSpeed);
		LeftBack.move(leftSpeed);

		delay(20);
	}
}
