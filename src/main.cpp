#include <string>
#include "UserDefined/functions.h"
#include "UserDefined/points.h"
#include "main.h"

void display()
{
	//thread to display important debug information
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	while(true) {
		pros::lcd::set_text(1, "X Position: " + std::to_string(robot_x));
		pros::lcd::set_text(2, "Y Position: " + std::to_string(robot_y));

		pros::delay(20);

		pros::lcd::clear();
	}
}

void initialize()
{
	pros::Task t(display); //start the display task

	reset();
}

void disabled() {}

void competition_initialize()
{
}

void autonomous()
{

}


void opcontrol() {
	stopThreads = true; //stop all threads that were running during auton (REMEMBER TO RESTART IMPORTANT ONES)

	while (true) {
		pros::delay(20);
	}
}
