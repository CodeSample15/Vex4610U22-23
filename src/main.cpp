/*
	Current problems:
		- Auto aiming still isn't 100% accurate after being pushed by accident
		- Auto aiming still needs a little bit of tweaking (save for final bot?)

		- Position tracking preset start locations are still blank

		- Flywheel auto spinup isn't tuned

*/

#include <string>
#include <iostream>
#include "UserDefined/methods.h"
#include "UserDefined/points.h"
#include "UserDefined/Replay.h"
#include "Screen/styles.hpp"
#include "Screen/firstPage.hpp"
#include "Screen/autonSelectionPage.hpp"
#include "Screen/done.hpp"
#include "autons.hpp"
#include "robot.h"
#include "main.h"

using namespace pros;

void display()
{
	//thread to display important debug information
	lcd::initialize();
	std::cout << "Running display thread" << std::endl;

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

bool recording;
void record()
{
	Replay replay(50);
	recording = true;

	while(recording)
	{
		replay.record();
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
	Task x(record); //instant replay

	//for driver testing
	Task f(controller_display);
	Task t(display);


	init(); //all the initialization will happen in the methods file
}

void disabled() {}

void competition_initialize()
{
	Task f(controller_display);

	//only start first page gui during competition
	load_autons(a_manager);
	init_styles();
	firstPage(); //auton selection (calls the init function)
}

void autonomous()
{
	if(selectedAuton != -1)
	{
		autonStarted = true;
		runningAuton = true;
		a_manager.runAuton(selectedAuton); //selectedAuton is from the autonSelectionPage files
		runningAuton = false;
	}
	else {
		std::cout << "Auton not selected! Not running anything" << std::endl;
	}
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
			curvedTurn = pow(curvedTurn, 2); //curvedTurn^2
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
			int targetRot = pos.angleTo(target_pos); //target pos is a predetermined Point object with the location of the goal

			//turn while button is still pressed
			TurnToRotation(turnPid, targetRot, 1, [](){ return controller.get_digital(E_CONTROLLER_DIGITAL_A)==1; });
		}

		//driving the intake
		if(controller.get_digital(E_CONTROLLER_DIGITAL_L1) == 1)
			IntakeOne.move(127);
		else if(controller.get_digital(E_CONTROLLER_DIGITAL_L2) == 1)
			IntakeOne.move(-127);
		else
			IntakeOne.brake();

		if(controller.get_digital(E_CONTROLLER_DIGITAL_R1) == 1)
			IntakeTwo.move(127);
		else if(controller.get_digital(E_CONTROLLER_DIGITAL_R2) == 1)
			IntakeTwo.move(-127);
		else
			IntakeTwo.brake();

		//indexer
		if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_UP))
			shoot(true);
		if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN))
			indexerBack(); //change this in the future so that the indexing is less manual

		pros::delay(20);
	}
}
