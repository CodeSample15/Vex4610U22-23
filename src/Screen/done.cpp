#include "done.hpp"
#include "styles.hpp"
#include "autonSelectionPage.hpp"
#include "UserDefined/methods.h"
#include "robot.h"

void doneScreen() //thread that runs until completion of the auton
{
    lv_obj_t * status;

    while(true) {
        if(autonStarted)
            break;

        lv_obj_clean(lv_scr_act());

        if(selectedAuton == -1)
            status = makeLabel(lv_scr_act(), 0, 0, 100, 50, "No Auton", &text_red_style);
        else if(gyro2.is_calibrating() || gyro.is_calibrating())
            status = makeLabel(lv_scr_act(), 0, 0, 100, 50, "Auton selected (Calibrating...)", &text_blue_style);
        else
            status = makeLabel(lv_scr_act(), 0, 0, 100, 50, "Auton selected (Ready to go)", &text_white_style);

        lv_obj_align(status, NULL, LV_ALIGN_CENTER, 0, 0);

        pros::delay(100);
    }

    //display gauges
    static lv_color_t needle_colors[1];
    needle_colors[0] = LV_COLOR_CYAN;

    lv_obj_clean(lv_scr_act());

    //flywheel gauge for speed and temperature
    lv_obj_t * flyWheelGauge = makeGauge(lv_scr_act(), 0, 0, 200, 200, 0, 600, 180, 20, 5, 1, needle_colors);
    lv_gauge_set_critical_value(flyWheelGauge, 500);
    lv_obj_align(flyWheelGauge, NULL, LV_ALIGN_CENTER, 0, 0);

    while(true) {
        lv_gauge_set_value(flyWheelGauge, 0, FlyWheel.get_actual_velocity());

        pros::delay(20);
    }
}