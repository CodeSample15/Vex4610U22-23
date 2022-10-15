/*
	Current problems:
		- Position tracking with inertial is too drifty. Switch to complete tracking with rotation sensors ASAP		(will be fixed)
		- Jitter still exists for auto aiming 																		(fixed)
		- X-axis wheel is spinning when turning (might be an issue, should be tested more)							(to be tested next class)
		- Turning is on too much of a curve (dampen? put on a lesser curve?)										(fixed)
		- Still no way to tell gyro is calibrating (save for last)
*/

#include <string>
#include <iostream>
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
		lcd::set_text(0, "X Position: " + std::to_string(robot_x));
		lcd::set_text(1, "Y Position: " + std::to_string(robot_y));
		lcd::set_text(2, "Rotation: " + std::to_string(gyro.get_rotation()));
		lcd::set_text(3, "Other rotation: " + std::to_string(gyro2.get_rotation()));
		lcd::set_text(4, "Local rotation " + std::to_string(getRegularRotation()));
		lcd::set_text(5, "xOdom: " + std::to_string(xEncoder.get_position()));
		lcd::set_text(6, "yOdom: " + std::to_string(yEncoder.get_position()));

		pros::delay(20);

		lcd::clear();
	}
}

void controller_display()
{
	controller.clear();

	while(true) {
		controller.set_text(0, 0, "X: " + std::to_string((int)robot_x) + "  Y: " + std::to_string((int)robot_y));

		pros::delay(60);
	}
}

void initialize()
{
	Task d(display);
	Task f(controller_display);
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
	double curvedTurn; //putting the turn on a curve
	bool negative; //for after the turn value is squared

	RightFront.set_brake_mode(E_MOTOR_BRAKE_COAST);
	RightBack.set_brake_mode(E_MOTOR_BRAKE_COAST);
	LeftFront.set_brake_mode(E_MOTOR_BRAKE_COAST);
	LeftBack.set_brake_mode(E_MOTOR_BRAKE_COAST);

	while (true) {

		//auto aim button (hold to activate)
		if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_A) == 0) {
			//regular drive code

			curvedTurn = controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) / 127.0; //mapping the input betweeen 0 and 1;
			negative = curvedTurn < 0;
			curvedTurn *= curvedTurn; //curvedTurn^3
			curvedTurn *= 127 * (negative && curvedTurn > 0 ? -1 : 1); //returning the output value to the desired range

			rightSpeed = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) - (int)curvedTurn * 1.5;
			leftSpeed = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) + (int)curvedTurn * 1.5;

			RightFront.move(rightSpeed);
			RightBack.move(rightSpeed);
			LeftFront.move(leftSpeed);
			LeftBack.move(leftSpeed);
		}
		else {
			//calculate angle to the goal x y point (hardcoded)
			Points pos(robot_x, robot_y);
			int targetRot = pos.angleTo(target_pos);

			//excecute turn as long as the turn button is pressed
			std::cout << "Turning to rotation: ";
			std::cout << std::to_string(targetRot) << std::endl;

			TurnToRotation(turnPid, targetRot, 1, [](){ return controller.get_digital(E_CONTROLLER_DIGITAL_A)==1; });
		}

		pros::delay(20);
	}
}
