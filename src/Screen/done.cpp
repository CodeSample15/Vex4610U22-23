#include "done.hpp"
#include "styles.hpp"
#include "autonSelectionPage.hpp"
#include "UserDefined/methods.h"
#include "robot.h"

void doneScreen() //thread that runs until completion of the auton
{
    lv_obj_t * status;

    while(true) {
        if(!runningAuton && autonStarted)
            break;

        lv_obj_clean(lv_scr_act());

        if(autonStarted)
            status = makeLabel(lv_scr_act(), 0, 0, 100, 50, "Running", &text_green_style);
        else if(selectedAuton == -1)
            status = makeLabel(lv_scr_act(), 0, 0, 100, 50, "No auton selected!", &text_red_style);
        else if(gyro2.is_calibrating() || gyro.is_calibrating())
            status = makeLabel(lv_scr_act(), 0, 0, 100, 50, "Auton selected (Calibrating...)", &text_blue_style);
        else
            status = makeLabel(lv_scr_act(), 0, 0, 100, 50, "Auton selected (Ready to go)", &text_white_style);

        lv_obj_align(status, NULL, LV_ALIGN_CENTER, 0, 0);

        pros::delay(100);
    }
}