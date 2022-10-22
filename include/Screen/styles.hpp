#include "main.h"

extern lv_style_t posButtonREL;
extern lv_style_t posButtonPRES;

void init_styles();
lv_obj_t * makeButton(int id, lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, const char * title, lv_style_t * styleREL, lv_style_t * stylePR);
lv_obj_t * makeLabel(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, const char * title, lv_style_t * style);