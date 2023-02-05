#include "done.hpp"
#include "main.h"
#include "robot.h"
#include "UserDefined/methods.h"
#include "Screen/autonSelectionPage.hpp"
#include "Screen/firstPage.hpp"
#include "Screen/styles.hpp"

E_START_POS start_pos;

static lv_res_t btn_click_action(lv_obj_t * btn) 
{
    uint8_t id = lv_obj_get_free_num(btn);

    switch(id) 
    {
        case 0:
            start_pos = ONE;
            break;

        case 1:
            start_pos = TWO;
            break;

        case 2:
            start_pos = THREE;
            break;

        case 3:
            start_pos = FOUR;
            break;

        case 4:
            start_pos = FIVE;
            break;
    }


    a_select(-1);
    return LV_RES_OK;
}

static lv_res_t skills_btn_action(lv_obj_t * btn) { //for skills button
    RunningSkills = true;
    pros::Task e(doneScreen);
    init();

    return LV_RES_OK;
}

lv_res_t toFirstPage(lv_obj_t* obj)
{
    firstPage();
    return LV_RES_OK;
}

//determine start position of the robot for x and y offsets
void firstPage()
{
    lv_obj_clean(lv_scr_act()); //wipe the screen clear

    int buttonWidth = 100;
    int buttonHeight = 50;

    lv_obj_t * title = makeLabel(lv_scr_act(), 0, 0, buttonWidth, buttonHeight, "Choose starting location:", nullptr);
    lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    lv_obj_t * goalInd = makeLabel(lv_scr_act(), 0, 0, 10, 10, "Goal", &text_red_style);
    lv_obj_align(goalInd, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 30, -15);

    lv_obj_t * goalInd2 = makeLabel(lv_scr_act(), 0, 0, 10, 10, "Goal", &text_blue_style);
    lv_obj_align(goalInd2, NULL, LV_ALIGN_IN_TOP_RIGHT, -25, 25);

    lv_obj_t * skillsButton = makeButton(0, lv_scr_act(), 230, 75, buttonWidth, buttonHeight, "Skills", &blueButtonREL, &blueButtonPRES);

    lv_btn_set_action(skillsButton, LV_BTN_ACTION_CLICK, skills_btn_action);

    lv_obj_t * button1 = makeButton(0, lv_scr_act(), 10, 10, buttonWidth, buttonHeight, "Pos 1", &posButtonREL, &posButtonPRES);
    lv_obj_t * button2 = makeButton(1, lv_scr_act(), 10, 75, buttonWidth, buttonHeight, "Pos 2", &posButtonREL, &posButtonPRES);
    lv_obj_t * button3 = makeButton(2, lv_scr_act(), 10, 140, buttonWidth, buttonHeight, "Pos 3", &posButtonREL, &posButtonPRES);
    
    lv_obj_t * button4 = makeButton(3, lv_scr_act(), 120, 190, buttonWidth, buttonHeight, "Pos 4", &posButtonREL, &posButtonPRES);
    lv_obj_t * button5 = makeButton(4, lv_scr_act(), 230, 190, buttonWidth, buttonHeight, "Pos 5", &posButtonREL, &posButtonPRES);

    lv_btn_set_action(button1, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_action(button2, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_action(button3, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_action(button4, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_action(button5, LV_BTN_ACTION_CLICK, btn_click_action);
}