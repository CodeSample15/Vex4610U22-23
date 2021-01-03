/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Student                                          */
/*    Created:      Tue Dec 22 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// BRight               motor         5
// BLeft                motor         6
// FRight               motor         4
// FLeft                motor         3
// RIntake              motor         14
// LIntake              motor         15
// FRoller              motor         9
// BRoller              motor         10
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int orientation=0;
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  while (true) {
    if(Controller1.ButtonX.pressing()){
      orientation++;
    }
    if (abs(Controller1.Axis3.position(percent)) > 10) {
      if(abs(Controller1.Axis1.position(percent)) > 10){ //strafing
        if(Controller1.Axis3.position(percent) > 10&&Controller1.Axis1.position(percent) > 10){
          FLeft.setVelocity(Controller1.Axis3.position(), percent);
          FRight.setVelocity(Controller1.Axis3.position()/2, percent);
          BLeft.setVelocity(Controller1.Axis3.position(), percent);
          BRight.setVelocity(0, percent);
        }
        
        if(Controller1.Axis3.position(percent) > 10&&Controller1.Axis1.position(percent) < 10){
          FRight.setVelocity(Controller1.Axis3.position(), percent);
          FLeft.setVelocity(Controller1.Axis3.position()/2, percent);
          BRight.setVelocity(Controller1.Axis3.position(), percent);
          BLeft.setVelocity(0, percent);
        }
        
        if(Controller1.Axis3.position(percent) < 10&&Controller1.Axis1.position(percent) < 10){
          FRight.setVelocity(Controller1.Axis3.position(), percent);
          FLeft.setVelocity(Controller1.Axis3.position()/2, percent);
          BRight.setVelocity(Controller1.Axis3.position(), percent);
          BLeft.setVelocity(0, percent);
        }

        if(Controller1.Axis3.position(percent) < 10&&Controller1.Axis1.position(percent) > 10){
          FLeft.setVelocity(Controller1.Axis3.position(), percent);
          FRight.setVelocity(Controller1.Axis3.position()/2, percent);
          BLeft.setVelocity(Controller1.Axis3.position(), percent);
          BRight.setVelocity(0, percent);
        }
      } //end of strafing
      else{
      FLeft.setVelocity(Controller1.Axis3.position(), percent);
      FRight.setVelocity(Controller1.Axis3.position(), percent);
      BLeft.setVelocity(Controller1.Axis3.position(), percent);
      BRight.setVelocity(Controller1.Axis3.position(), percent);
      }
    }
    else if (abs(Controller1.Axis1.position(percent)) > 10) {
      FLeft.setVelocity(Controller1.Axis1.position(), percent);
      FRight.setVelocity(-Controller1.Axis1.position(), percent);
      BLeft.setVelocity(Controller1.Axis1.position(), percent);
      BRight.setVelocity(-Controller1.Axis1.position(), percent);
    }
    else {
      FLeft.setVelocity(0, percent);
      FRight.setVelocity(0, percent);
      BLeft.setVelocity(0, percent);
      BRight.setVelocity(0, percent);
    }

    if(Controller1.ButtonL2.pressing()){
      RIntake.setVelocity(100,percent);
      LIntake.setVelocity(100,percent);
    }
    else if(Controller1.ButtonL1.pressing()){
      RIntake.setVelocity(-100,percent);
      LIntake.setVelocity(-100,percent);
    }
    else{
      RIntake.setVelocity(0,percent);
      LIntake.setVelocity(0,percent);
    }

    if(Controller1.ButtonR2.pressing()){
      FRoller.setVelocity(100,percent);
      BRoller.setVelocity(100,percent);
    }
    else if(Controller1.ButtonR1.pressing()){
      FRoller.setVelocity(-100,percent);
      BRoller.setVelocity(-100,percent);
    }
    else{
      FRoller.setVelocity(0,percent);
      BRoller.setVelocity(0,percent);
    }
    FRoller.spin(forward);
    BRoller.spin(forward);
    RIntake.spin(forward);
    LIntake.spin(forward);
    FLeft.spin(forward);
    FRight.spin(forward);
    BLeft.spin(forward);
    BRight.spin(forward);
  }
}
