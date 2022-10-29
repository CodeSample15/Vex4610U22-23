#pragma once
#include "main.h"

enum E_START_POS {ONE, TWO, THREE, FOUR, FIVE};
extern E_START_POS start_pos;

void firstPage();
lv_res_t toFirstPage(lv_obj_t* obj);