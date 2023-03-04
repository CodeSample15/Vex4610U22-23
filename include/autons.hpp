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
    //get first roller
    pros::Task f_thread(flyWheelThread);

    set_drive_raw(-90, -30); //roller sticks out on the right so drive more on the left
    Roller.move_velocity(100);
    pros::delay(350);
    Roller.brake();
    stop_drive();

    pros::delay(500);

    curRot = gyro.get_rotation();

    //turn to nearest ring
    Move(movePid, 400, 0.9);
    Turn(turnPid, -45, 0.9);

    flyWheelSpeed = 370; //spin up flywheel

    //turn on intake and drive
    inTake();
    MoveUntilLine(40); //for consistency
    Move(movePid, turnPid, 100, 1);

    //turn to roller
    TurnTo(turnPid, 90, 0.5);

    //back up and spin roller
    Move(movePid, turnPid, -1100, 0.6, 2.5);
    set_drive_raw(-90, -50);
    Roller.move_velocity(100);
    pros::delay(400);
    Roller.brake();
    stop_drive();

    pros::delay(500);
    curRot = gyro.get_rotation();

    stopIntake();

    //move away from roller and turn to shoot
    MoveUntilLine(60);
    TurnTo(turnPid, 10, 0.7);

    //shoot
    outTake();
    pros::delay(3000);
    stopIntake();

    TurnTo(turnPid, 50, 1);
    Strings.set_value(1);
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
