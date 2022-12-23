#include "main.h"

extern lv_style_t posButtonREL, posButtonPRES;
extern lv_style_t redButtonREL, redButtonPRES;
extern lv_style_t blueButtonREL, blueButtonPRES;
extern lv_style_t whiteButtonREL, whiteButtonPRES;

extern lv_style_t text_white_style, text_blue_style, text_red_style, text_green_style;

void init_styles();
lv_obj_t * makeButton(int id, lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, const char * title, lv_style_t * styleREL, lv_style_t * stylePR);
lv_obj_t * makeLabel(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, const char * text, lv_style_t * style);
lv_obj_t * makeGauge(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, int32_t min, int32_t max, uint16_t angle, uint8_t line_count, uint8_t label_count, uint8_t needle_count, lv_color_t needle_colors[]);