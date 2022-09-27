/*
	Current problems:
		- Jitter still exists for auto aiming
		- Points class doesn't correctly calculate angleTo another point
		- X-axis wheel is spinning when turning (might be an issue, should be tested more)
		- Turning is on too much of a curve (dampen? put on a lesser curve?)
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
		lcd::set_text(3, "xOdom: " + std::to_string(xEncoder.get_position()));
		lcd::set_text(3, "yOdom: " + std::to_string(yEncoder.get_position()));
		//lcd::set_text(3, "Right front temp: " + std::to_string(RightFront.get_temperature()));
		//lcd::set_text(4, "Right back temp: " + std::to_string(RightBack.get_temperature()));
		lcd::set_text(5, "Left front temp: " + std::to_string(LeftFront.get_temperature()));
		lcd::set_text(6, "Left back temp: " + std::to_string(LeftBack.get_temperature()));

		pros::delay(20);

		lcd::clear();
	}
}

void initialize()
{
	Task d(display);
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
			curvedTurn *= curvedTurn;
			curvedTurn *= 127 * (negative ? -1 : 1); //returning the output value to the desired range

			rightSpeed = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) - (int)curvedTurn;
			leftSpeed = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) + (int)curvedTurn;

			RightFront.move(rightSpeed);
			RightBack.move(rightSpeed);
			LeftFront.move(leftSpeed);
			LeftBack.move(leftSpeed);

			double speed = RightFront.get_actual_velocity() * 2 * 3.1415926 * 1.625;
			if(abs(speed) > abs(maxSpeed)) {
				maxSpeed = speed;
			}
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
