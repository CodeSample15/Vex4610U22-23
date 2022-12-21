#pragma once
#include "UserDefined/methods.h"
#include "UserDefined/AutonManager.h"

void load_autons(AutonManager& manager);

inline void empty() {

}

//add autons here
inline void roll()
{
    Move(movePid, -100, 1);
    spinRollerToColor(TEAM_COLOR);
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

    manager.addAuton(&empty, "No Auton", "Run when all is lost.", true, true, true, true, true);
    manager.addAuton(&roll, "Roll", "Drive up to roller and spin until color matches.", true, false, false, false, false);
}