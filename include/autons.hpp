#pragma once
#include "UserDefined/methods.h"
#include "robot.h"
#include "UserDefined/AutonManager.h"

void load_autons(AutonManager& manager);

inline void empty() {

}

//add autons here
inline void roll()
{
    set_drive_raw(-70, -70);

    Roller.move_velocity(100);

    pros::delay(400);

    Roller.brake();

    stop_drive();

    Move(movePid, 500, 1);
}

inline void skills() {
    //roll first roller
    set_drive_raw(-70, -70);

    Roller.move_velocity(-100);

    pros::delay(300);

    Roller.brake();

    stop_drive();

    Move(movePid, 1500, 1);

    //turn to second roller
    Turn(turnPid, 90, 1);

    //move back to second roller
    Move(movePid, turnPid, -1500, 1);

    //move the rest of the way using raw values for the motors
    set_drive_raw(-70, -70);

    //turn the thing
    Roller.move_velocity(-50);

    pros::delay(700);

    Roller.brake();
    stop_drive();

    //move forward away from the wall
    Move(movePid, 1000, 1);
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
}