/*
	Current problems:
		- Preloads auton doesn't work (PID is acting funky)
		- Move function is non functional (might be fixed)
		- Reset button doesn't work as intented
		- Position tracking might be out of wack again
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

bool competing = false;
bool autoFlyWheelSpeedMode = false; //for driver control

void display()
{
	//thread to display important debug information
	lcd::initialize();
	std::cout << "Running display thread" << std::endl;

	while(true) {
		lcd::set_text(0, "X Position: " + std::to_string(robot_x));
		lcd::set_text(1, "Y Position: " + std::to_string(robot_y));
		lcd::set_text(2, "Rotation: " + std::to_string(gyro.get_rotation()));
		lcd::set_text(3, "Blue: " + std::to_string(optical.get_rgb().blue));
		lcd::set_text(4, "Red: " + std::to_string(optical.get_rgb().red));
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
		controller.set_text(1,0,"Rot: " + std::to_string(gyro.get_rotation()));
		pros::delay(60);
		controller.set_text(2, 0, "Flywheel temp: " + std::to_string((int)FlyWheel.get_temperature()));
		pros::delay(60);
	}
}

void initialize() {}

void disabled() {}

void competition_initialize()
{
	Task f(controller_display);
	competing = true;
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
		a_manager.runAuton(selectedAuton); //selectedAuton is from the autonSelectionPage files
	}
	else if(RunningSkills) {
		skills(); //in the autons.hpp file
	}
	else {
		std::cout << "Auton not selected! Not running anything" << std::endl;
	}
}

//in a separate thread to allow time for the piston to fully retract without pausing the main drive code
void shootThread() {
	while(true) {
		//indexer
		if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_R1))
			shoot(true);
		if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN))
			indexerBack(); //change this in the future so that the indexing is less manual

		delay(10);
	}
}

/*
	Both the string launcher and the reset position system require a button to be held for a certain amount of time. For this reason, I've placed them into a different thread to prevent them from stopping the main drive code from running

	Since the two buttons are most likely never going to be pressed at the same time (because why would they ever), I've placed the functions into the same thread.
	Is this bad practice? Maybe. But I don't care.
*/
void stringsAndResetThread() {
	int count = 0; //excuse the bad naming, but they both do the same thing, just for seperate buttons
	int otherCount = 0;

	while(true) {
		//STRINGS
		if(controller.get_digital(E_CONTROLLER_DIGITAL_UP)) {
			controller.rumble(".");
			pros::delay(500);
			count++;

			if(count == 3 && controller.get_digital(E_CONTROLLER_DIGITAL_UP))
				Strings.set_value(1);
		}
		else {
			count = 0;
		}

		//RESET POSITION
		if(controller.get_digital(E_CONTROLLER_DIGITAL_LEFT) && otherCount != -1) {
			pros::delay(1000);
			otherCount++;

			if(otherCount == 2 && controller.get_digital(E_CONTROLLER_DIGITAL_LEFT)) {
				reset();
				controller.rumble("-"); //long rumble to tell driver the position was reset
				otherCount = -1;
			}
		}
		else if (otherCount != 0){
			otherCount = 0;
		}
		else if(controller.get_digital(E_CONTROLLER_DIGITAL_LEFT) == 0) {
			otherCount = 0;
		}
		

		pros::delay(10);
	}
}

void opcontrol() {

	/*
		Current controls:

		Forward/Backward:              Left analog stick
		Right/Left:                    Right analog stick
		Auto aim:                      A
		Shoot:                         R1
		Retract piston:                Down arrow
		Intake/Outtake:                L1/L2
		Flywheel to constant speed:    X
		Flywheel off:                  B
		Flywheel automatic modeL       Y
		Spin Roller:                   R2
		Expand (strings):              Up arrow (hold)
		Reset position:                Left arrow (hold)

	    Open button(s):
			* Right arrow
	*/



	if(!competing) {
		Task x(record); //instant replay

		//for driver testing
		Task f(controller_display);
		Task t(display);


		init(); //all the initialization will happen in the methods file
	}

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

	pros::Task shoot(shootThread);
	pros::Task s(stringsAndResetThread);

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
			int targetRot = getPositionXY().angleTo(target_pos); //target pos is a predetermined Point object with the location of the goal

			//turn while button is still pressed
			TurnToRotation(turnPid, targetRot, 1, [](){ return controller.get_digital(E_CONTROLLER_DIGITAL_A)==1; });
		}

		//driving the intake
		if(controller.get_digital(E_CONTROLLER_DIGITAL_L1) == 1) {
			IntakeOne.move(127);
			IntakeTwo.move(127);
		}
		else if(controller.get_digital(E_CONTROLLER_DIGITAL_L2) == 1) {
			IntakeOne.move(-127);
			IntakeTwo.move(-127);
		}
		else {
			IntakeOne.brake();
			IntakeTwo.brake();
		}

		if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) {
			FlyWheel.move_velocity(440);
			flyWheelSpeed = 440;
			autoFlyWheelSpeedMode = false;
		}
		else if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
			spinDown();
			autoFlyWheelSpeedMode = false;
		}
		else if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)) {
			autoFlyWheelSpeedMode = true;
		}


		if(autoFlyWheelSpeedMode) {
			spinUp(); //constantly adjust speed
		}


		if(controller.get_digital(E_CONTROLLER_DIGITAL_R2))
			Roller.move_velocity(100);
		else
			Roller.brake();

		//update the indicator to let the driver know visually if the flywheel is up to speed
		if(!flyRecovering())
			optical.set_led_pwm(100);
		else
			optical.set_led_pwm(0);

		pros::delay(20);
	}
}