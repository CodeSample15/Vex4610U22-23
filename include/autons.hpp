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
}


inline void skills() {
    gyro.tare();

    set_drive_raw(-170, 0);
    Roller.move_velocity(100);
    pros::delay(450);
    Roller.brake(); //roll
    stop_drive();

    Move(movePid, 500, 1);
    Turn(turnPid, -90, 1);
    Move(movePid, 500, 1); //hopefully at 2nd roller

    //set_drive_raw(-170, -170);
    //Roller.move_velocity(100);
    //pros::delay(500);
    //pRoller.brake(); //roll
    /*
    Move(movePid, -10, 1);
    Turn(turnPid, 90, 1);
    Move(movePid, 2000, 1); //go through low goal

    Turn(turnPid, 90, 1);
    Move(movePid, 2000, 1); //hopefully at roller

    Turn(turnPid, -90, 1);

    set_drive_raw(-170, -170);
    Roller.move_velocity(100);
    pros::delay(500);
    Roller.brake(); //roll

    Move(movePid, -10, 1);
    Turn(turnPid, -90, 1);
    Move(movePid, 500, 1); //hopefully at 2nd roller

    set_drive_raw(-170, -170);
    Roller.move_velocity(100);
    pros::delay(500);
    Roller.brake(); //roll

    Move(movePid, -200, 1); //cover tiles
    Turn(turnPid, -135, 1);
    //Strings.set_value(1);
    */
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