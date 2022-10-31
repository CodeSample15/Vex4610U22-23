#pragma once
#include "UserDefined/AutonManager.h"

void load_autons(AutonManager& manager);

//add autons here
void auton1()
{

}


void load_autons(AutonManager& manager) 
{
    /*
        Position Guide:
        1
        2
        3
          4  5
    */

    manager.addAuton(&auton1, "Test", "This is a test auton to make sure the selection menu works. Doesn't actually do anything.", false, false, false, true, false);
}