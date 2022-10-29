#include "main.h"
#include "Screen/autonSelectionPage.hpp"
#include "Screen/styles.hpp"

lv_res_t to_a_select(lv_obj_t * obj) 
{
    a_select();
    return LV_RES_OK;
}

void a_select()
{
    lv_obj_clean(lv_scr_act());

    lv_obj_t * page = lv_page_create(lv_scr_act(), NULL);
    
    lv_obj_t * but = makeButton(0, page, 0, 0, 100, 50, "test", &redButtonREL, &redButtonPRES);
}