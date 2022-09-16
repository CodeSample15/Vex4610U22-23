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
	bool negative; //for after I square the value

	RightFront.set_brake_mode(E_MOTOR_BRAKE_COAST);
	RightBack.set_brake_mode(E_MOTOR_BRAKE_COAST);
	LeftFront.set_brake_mode(E_MOTOR_BRAKE_COAST);
	LeftBack.set_brake_mode(E_MOTOR_BRAKE_COAST);

	while (true) {
		if(!controller.get_digital(E_CONTROLLER_DIGITAL_A)) { //auto aim button (hold to activate)
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
		}
		else {
			//calculate angle to the goal x y point (hardcoded)
			Points pos(robot_x, robot_y);
			int targetRot = pos.angleTo(target_pos);

			//excecute turn as long as the turn button is pressed
			TurnToRotation(turnPid, targetRot, 1, [](){ return controller.get_digital(E_CONTROLLER_DIGITAL_A)==1; });
		}

		delay(20);
	}
}
