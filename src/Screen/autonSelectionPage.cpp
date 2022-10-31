#include "main.h"
#include "UserDefined/methods.h"
#include "Screen/autonSelectionPage.hpp"
#include "Screen/styles.hpp"

static lv_res_t btn_click_action(lv_obj_t * btn) 
{
    uint8_t id = lv_obj_get_free_num(btn);

    return LV_RES_OK;
}

lv_res_t to_a_select(lv_obj_t * obj) 
{
    a_select();
    return LV_RES_OK;
}

void a_select()
{
    lv_obj_clean(lv_scr_act());

    lv_obj_t * page = lv_page_create(lv_scr_act(), NULL);
    
    for(int i=0; i<a_manager.numAutons(); i++) {
        lv_obj_t * but = makeButton(i, page, 0, 0, 300, 50, a_manager.getName(i).c_str(), &redButtonREL, &redButtonPRES);
        lv_btn_set_action(but, LV_BTN_ACTION_CLICK, btn_click_action);
    }
}