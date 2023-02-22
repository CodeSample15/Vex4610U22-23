#pragma once
#include "UserDefined/methods.h"
#include "UserDefined/AutonManager.h"
#include "Screen/firstPage.hpp"
#include "robot.h"

void load_autons(AutonManager& manager);

inline void empty() {

}

//add autons here
inline void roll()
{
    set_drive_raw(-170, -170);

    Roller.move_velocity(100);

    pros::delay(500);

    Roller.brake();

    stop_drive();

    Move(movePid, 500, 1);
}

inline void roll_and_shoot() 
{
    spinUp();
    roll();
    Turn(turnPid, 90, 1);
    IntakeOne.move(127);
    IntakeTwo.move(127);
}

inline void shoot_low() {
    spinUp();
    Move(movePid, 500, 1);

    if(start_pos == ONE || start_pos==TWO || start_pos==THREE) {
        Turn(turnPid, 100, 1); //shoot into net to score low goal
    }
    else {
        Turn(turnPid, -100, 1);
    }

    IntakeOne.move(127);
    IntakeTwo.move(127);
}


inline void skills() {
    //spin up flywheel
    spinPrep();
    pros::delay(500);
    spinUp();

    //get first roller
    set_drive_raw(-190, -50); //roller sticks out on the right so drive more on the left
    Roller.move_velocity(50);
    pros::delay(700);
    Roller.brake();
    stop_drive();

    //turn towards nearest disk
    //we get 2 preloads so just intake one
    Move(movePID, 100, 1);
    Turn(turnPID, -90, 1) //aiming for 2nd diagonal disc
    
    //intake disk
    IntakeOne.move(-127);
	IntakeTwo.move(-127);
    Move(movePID, 300, 1);

    //turn towards next roller
    Turn(turnPID, -180, 1);
    Move(turnPID, -300, 1);
    IntakeOne.brake(); //enough intaking
	IntakeTwo.brake();

    //get second roller
    set_drive_raw(-190, -50);
    Roller.move_velocity(50);
    pros::delay(700);
    Roller.brake();
    stop_drive();

    //turn towards high goal
    Move(movePID, 100, 1);
    Turn(turnPID, 180, 1);

    //shoot single disk
    for(int i=0; i<3; i++){ // maybe move forward towards goal if discs are not shooting far enough
        IntakeOne.move(127);
	    IntakeTwo.move(127);
        pros::delay(200);
        IntakeOne.brake();
	    IntakeTwo.brake();
        pros::delay(500);
    }

    //move forward
    //turn towards row of 3 disks

    //Not too sure what I'm supposed to do here
    //I am picking up 3 discs, shooting, going to the other half of the field, picking up, shooting
    //In an "S" pattern to score 3 in red high goal and 3 in blue high goal
    Turn(turnPID, -45, 1);

    //gather first three disks
    IntakeOne.move(-127);
	IntakeTwo.move(-127);
    Move(movePID, 1000, 1);

    //turn towards high goal
    Turn(turnPID, -90, 1);
    IntakeOne.brake();
	IntakeTwo.brake();

    //shoot
    for(int i=0; i<3; i++){
        IntakeOne.move(127);
	    IntakeTwo.move(127);
        pros::delay(200);
        IntakeOne.brake();
	    IntakeTwo.brake();
        pros::delay(500);
    }

    //turn 180 degrees
    Turn(turnPID, 180, 1);

    //move to other side of 3 disks
    IntakeOne.move(-127);
	IntakeTwo.move(-127);
    Move(movePID, 500, 1);

    //turn and gather next 3 disks
    Turn(turnPID, -90, 1);
    Move(movePID, 500, 1);

    //turn to high goal and shoot
    Turn(turnPID, 120, 1); //weird angle so this might not be right
    for(int i=0; i<3; i++){
        IntakeOne.move(127);
	    IntakeTwo.move(127);
        pros::delay(200);
        IntakeOne.brake();
	    IntakeTwo.brake();
        pros::delay(500);
    }

    //move to the next roller
    Turn(turnPID, -120, 1); //negative of weird angle
    Move(movePID, 200, 1); //move a small bit to not disturb stack of 3
    Turn(turnPID, -45, 1);

    IntakeOne.move(-127);
	IntakeTwo.move(-127);
    Move(movePID, 200, 1); //collect 2nd disc in diagonal and fire ASAP to prevent misfire
    pros::delay(500);
    Turn(turnPID, 180, 1);

    IntakeOne.move(127);
	IntakeTwo.move(127);
    pros::delay(200);
    IntakeOne.brake();
	IntakeTwo.brake();

    //turn and roll the third roller
    Turn(turnPID, 90, 1);
    Move(movePID, 200, 1); //roller sticking out towards the corner

    set_drive_raw(-190, -50);
    Roller.move_velocity(50);
    pros::delay(700);
    Roller.brake();
    stop_drive();

    //move away from the roller and turn to the nearest disk
    //gather the disk and shoot into high goal
    //move and turn to next roller
    Move(movePID, 200, 1);
    Turn(turnPID, -90, 1);
    Move(movePID, -200, 1);

    //spin fourth roller
    set_drive_raw(-50, -190);
    Roller.move_velocity(50);
    pros::delay(700);
    Roller.brake();
    stop_drive();

    //move to diagonal

    //gather and shoot 3 stack

    //turn and continue to the next set of disks

    //shoot next 3 disks

    //continue to last 3 stack

    //gather and score 3 stack into high goals

    //move along diagonal a little more

    //turn so that strings point into the field

    //expand

    /*
    gyro.tare();

    //roll first roller
    set_drive_raw(-170, -170);

    Roller.move_velocity(50);

    pros::delay(700);

    Roller.brake();

    stop_drive();

    //move away from first roller
    Move(movePid, 1500, 1);

    //turn to second roller
    Turn(turnPid, 90, 1);

    //move back to second roller
    Move(movePid, turnPid, -1700, 1);

    //move the rest of the way using raw values for the motors
    set_drive_raw(-190, -190);

    //turn the thing
    Roller.move_velocity(-50);

    pros::delay(700);

    Roller.brake();
    stop_drive();

    //move forward away from the wall
    Move(movePid, 1000, 1);

    Turn(turnPid, -30, 1);
    pros::delay(500);

    //launch strings
    Strings.set_value(1);
    */
}

inline void load_autons(AutonManager& manager) 
{
    /*
        Position Guide:
        1
        2
        3
          4  5
    */

    manager.addAuton(&empty, "No Auton", "Run when all is lost", true, true, true, true, true);
    manager.addAuton(&roll, "Roll", "Spin roller to color and move forward.", true, false, false, false, false);
    manager.addAuton(&roll_and_shoot, "Roll&Shoot", "Spin roller, spin to low goal and shoot", true, false, false, false, false);
    manager.addAuton(&shoot_low, "Shoot low", "Turn to low goal from any position and shoot.", true, true, true, true, true);
}
