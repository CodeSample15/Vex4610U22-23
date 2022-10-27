#include "main.h"
#include "Screen/firstPage.hpp"
#include "Screen/styles.hpp"

static lv_res_t btn_click_action(lv_obj_t * btn) 
{
    uint8_t id = lv_obj_get_free_num(btn);

    switch(id) 
    {
        case 0:
            break;

        case 1:
            break;

        case 2:
            break;

        case 3:
            break;

        case 4:
            break;
    }

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
    lv_obj_align(title, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);

    lv_obj_t * button1 = makeButton(0, lv_scr_act(), 10, 200, buttonWidth, buttonHeight, "Pos 1", &posButtonREL, &posButtonPRES);
    lv_obj_t * button2 = makeButton(1, lv_scr_act(), 120, 200, buttonWidth, buttonHeight, "Pos 2", &posButtonREL, &posButtonPRES);
    lv_obj_t * button3 = makeButton(2, lv_scr_act(), 230, 200, buttonWidth, buttonHeight, "Pos 3", &posButtonREL, &posButtonPRES);

    lv_obj_t * button4 = makeButton(3, lv_scr_act(), 10, 140, buttonWidth, buttonHeight, "Pos 4", &posButtonREL, &posButtonPRES);
    lv_obj_t * button5 = makeButton(4, lv_scr_act(), 10, 90, buttonWidth, buttonHeight, "Pos 5", &posButtonREL, &posButtonPRES);

    lv_btn_set_action(button1, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_action(button2, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_action(button3, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_action(button4, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_action(button5, LV_BTN_ACTION_CLICK, btn_click_action);
}