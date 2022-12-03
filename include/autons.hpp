#pragma once
#include "UserDefined/methods.h"
#include "UserDefined/AutonManager.h"

void load_autons(AutonManager& manager);

//add autons here
void test()
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

    manager.addAuton(&test, "Test", "Test auton DO NOT RUN IN MATCHES", true, true, true, true, true);
}