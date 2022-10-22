#include "main.h"
#include "Screen/firstPage.hpp"
#include "Screen/styles.hpp"
#include <iostream>

static lv_res_t btn_click_action(lv_obj_t * btn) 
{
    uint8_t id = lv_obj_get_free_num(btn);

    if(id == 0) 
    {
        std::cout << "Working?" << std::endl;
    }

    return LV_RES_OK;
}

lv_res_t toFirstPage(lv_obj_t* obj)
{
    firstPage();
    return LV_RES_OK;
}

void firstPage() 
{
    lv_obj_clean(lv_scr_act()); //wipe the screen clear

    lv_obj_t * testButton = makeButton(0, lv_scr_act(), 100, 100, 100, 50, "Test", &posButtonPRES, &posButtonREL);
}