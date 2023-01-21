#include "main.h"
#include "UserDefined/methods.h" //for the init() function
#include "autonSelectionPage.hpp"
#include "firstPage.hpp"
#include "styles.hpp"
#include "done.hpp"

#include <string>
#include <iostream>

int selectedAuton = -1;

static lv_res_t btn_click_action(lv_obj_t * btn) 
{
    uint8_t id = lv_obj_get_free_num(btn);

    selectedAuton = id;
    a_select(id); //reload the page

    return LV_RES_OK;
}

static lv_res_t lock_btn_action(lv_obj_t * btn)
{  
    pros::Task e(doneScreen); //start the doneScreen task
    //init(); //initialize the robot

    return LV_RES_OK;
}

static lv_res_t set_team_color(lv_obj_t * btn) {
    uint8_t id = lv_obj_get_free_num(btn);

    TEAM_COLOR = (id==0 ? 'r' : 'b');
    a_select(selectedAuton);

    return LV_RES_OK;
}

lv_res_t to_a_select(lv_obj_t * obj)
{
    a_select(-1);
    return LV_RES_OK;
}

void a_select(int selected)
{
    lv_obj_clean(lv_scr_act());

    //for the title of the page
    std::string titleStr = "Autons for pos ";

    switch(start_pos) 
    {
        case ONE:
            titleStr += "1: ";
            break;

        case TWO:
            titleStr += "2: ";
            break;

        case THREE:
            titleStr += "3: ";
            break;

        case FOUR:
            titleStr += "4: ";
            break;

        case FIVE:
            titleStr += "5: ";
            break;
    }

    int b_width, b_height;
    b_width = (selected==-1 ? 400 : 100); //100
    b_height = 50;


    //drawing the page itself
    lv_obj_t * title = makeLabel(lv_scr_act(), 0, 0, 100, 50, titleStr.c_str(), nullptr);
    if(TEAM_COLOR == 'r')
        lv_label_set_style(title, &text_red_style);
    else
        lv_label_set_style(title, &text_blue_style);
    lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);


    if(selected!=-1) {
        //description of auton
        lv_obj_t * description = makeLabel(lv_scr_act(), 170, 50, 100, 50, "wwwwwwwwwwwwwwwwww", &text_red_style);
        lv_label_set_long_mode(description, LV_LABEL_LONG_BREAK);
        lv_label_set_text(description, a_manager.getDescription(selected).c_str());

        //home and lock buttons
        lv_obj_t * homeBtn = makeButton(0, lv_scr_act(), 190, 160, 100, 50, "Home", &whiteButtonREL, &whiteButtonPRES);
        lv_obj_t * lockBtn = makeButton(0, lv_scr_act(), 330, 160, 100, 50, "Lock", &whiteButtonREL, &whiteButtonPRES);

        lv_btn_set_action(homeBtn, LV_BTN_ACTION_CLICK, toFirstPage);
        lv_btn_set_action(lockBtn, LV_BTN_ACTION_CLICK, lock_btn_action);
    }
    else {
        //draw home button in corner of the screen
        lv_obj_t * homeBtn = makeButton(0, lv_scr_act(), 15, 200, 60, 30, "Home", &whiteButtonREL, &whiteButtonPRES);
        lv_btn_set_action(homeBtn, LV_BTN_ACTION_CLICK, toFirstPage);
    }

    //draw team color buttons
    lv_obj_t * redBtn = makeButton(0, lv_scr_act(), 75, 200, 50, 30, "Red", &redButtonREL, &redButtonPRES);
    lv_btn_set_action(redBtn, LV_BTN_ACTION_CLICK, set_team_color);

    lv_obj_t * blueBtn = makeButton(1, lv_scr_act(), 130, 200, 50, 30, "Blue", &blueButtonREL, &blueButtonPRES);
    lv_btn_set_action(blueBtn, LV_BTN_ACTION_CLICK, set_team_color);

    //making the actual page that will hold all of the autons
    lv_obj_t * page = lv_page_create(lv_scr_act(), NULL);
    lv_obj_set_x(page, 20);
    lv_obj_set_y(page, 40);
    lv_obj_set_width(page, b_width + 30);
    lv_obj_set_height(page, 150);

    //filling the page with autons that can run from the bot's current location
    for(int i=0; i<a_manager.numAutons(); i++) {
        bool p[5];
        a_manager.getPositions(i, p);

        //only draw the auton if it can start in one the position the user selected as the robot's start location
        if((p[0] && start_pos==ONE) || (p[1] && start_pos==TWO) || (p[2] && start_pos==THREE) || (p[3] && start_pos==FOUR) || (p[4] && start_pos==FIVE)) {
            lv_obj_t * but;

            if(i == selected)
                but = makeButton(i, page, 0, ((b_height+10) * i), b_width, b_height, a_manager.getName(i).c_str(), &redButtonREL, &redButtonPRES);
            else
                but = makeButton(i, page, 0, ((b_height+10) * i), b_width, b_height, a_manager.getName(i).c_str(), &blueButtonREL, &blueButtonPRES);
            lv_btn_set_action(but, LV_BTN_ACTION_CLICK, btn_click_action);
        }
    }
}