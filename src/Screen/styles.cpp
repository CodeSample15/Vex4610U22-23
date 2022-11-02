#include "Screen/styles.hpp"
#include <iostream>

lv_style_t posButtonPRES, posButtonREL;
lv_style_t redButtonREL, redButtonPRES;
lv_style_t blueButtonREL, blueButtonPRES;
lv_style_t whiteButtonREL, whiteButtonPRES;

lv_style_t text_white_style, text_blue_style, text_red_style;

void init_styles() 
{
    lv_style_copy(&posButtonREL, &lv_style_plain);
    posButtonREL.body.main_color = LV_COLOR_MAKE(150, 0, 0);
    posButtonREL.body.grad_color = LV_COLOR_MAKE(0, 0, 150);
    posButtonREL.body.radius = 0;
    posButtonREL.text.color = LV_COLOR_MAKE(255, 255, 255);

    lv_style_copy(&posButtonPRES, &lv_style_plain);
    posButtonPRES.body.main_color = LV_COLOR_MAKE(255, 0, 0);
    posButtonPRES.body.grad_color = LV_COLOR_MAKE(0, 0, 255);
    posButtonPRES.body.radius = 0;
    posButtonPRES.text.color = LV_COLOR_MAKE(0, 0, 0);



    lv_style_copy(&redButtonPRES, &lv_style_plain);
    redButtonPRES.body.main_color = LV_COLOR_MAKE(155, 0, 0);
    redButtonPRES.body.grad_color = LV_COLOR_MAKE(155, 0, 0);
    redButtonPRES.body.radius = 20;
    redButtonPRES.text.color = LV_COLOR_WHITE;

    lv_style_copy(&redButtonREL, &lv_style_plain);
    redButtonREL.body.main_color = LV_COLOR_MAKE(255, 0, 0);
    redButtonREL.body.grad_color = LV_COLOR_MAKE(255, 0, 0);
    redButtonREL.body.radius = 20;
    redButtonREL.text.color = LV_COLOR_WHITE;



    lv_style_copy(&blueButtonPRES, &lv_style_plain);
    blueButtonPRES.body.main_color = LV_COLOR_MAKE(0, 0, 155);
    blueButtonPRES.body.grad_color = LV_COLOR_MAKE(0, 0, 155);
    blueButtonPRES.body.radius = 20;
    blueButtonPRES.text.color = LV_COLOR_WHITE;

    lv_style_copy(&blueButtonREL, &lv_style_plain);
    blueButtonREL.body.main_color = LV_COLOR_MAKE(0, 0, 255);
    blueButtonREL.body.grad_color = LV_COLOR_MAKE(0, 0, 255);
    blueButtonREL.body.radius = 20;
    blueButtonREL.text.color = LV_COLOR_WHITE;



    lv_style_copy(&blueButtonPRES, &lv_style_plain);
    blueButtonPRES.body.main_color = LV_COLOR_MAKE(155, 155, 155);
    blueButtonPRES.body.grad_color = LV_COLOR_MAKE(155, 155, 155);
    blueButtonPRES.body.radius = 20;
    blueButtonPRES.text.color = LV_COLOR_BLACK;

    lv_style_copy(&blueButtonREL, &lv_style_plain);
    blueButtonREL.body.main_color = LV_COLOR_MAKE(255, 255, 255);
    blueButtonREL.body.grad_color = LV_COLOR_MAKE(255, 255, 255);
    blueButtonREL.body.radius = 20;
    blueButtonREL.text.color = LV_COLOR_BLACK;


    lv_style_copy(&text_white_style, &lv_style_plain);
    text_white_style.text.color = LV_COLOR_WHITE;

    lv_style_copy(&text_blue_style, &lv_style_plain);
    text_blue_style.text.color = LV_COLOR_BLUE;

    lv_style_copy(&text_red_style, &lv_style_plain);
    text_red_style.text.color = LV_COLOR_RED;
}

//helper methods for making UI
lv_obj_t * makeButton(int id, lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, const char * title, lv_style_t * styleREL, lv_style_t * stylePR)
{
    lv_obj_t * btn = lv_btn_create(parent, NULL);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_size(btn, width, height);
    lv_obj_set_free_num(btn, id);
    lv_btn_set_style(btn, LV_BTN_STYLE_REL, styleREL);
    lv_btn_set_style(btn, LV_BTN_STYLE_PR, stylePR);

    lv_obj_t * label = lv_label_create(btn, NULL);
    lv_label_set_text(label, title);
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

    return btn;
}

lv_obj_t * makeLabel(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, const char * text, lv_style_t * style)
{
    lv_obj_t * label = lv_label_create(parent, NULL);
    lv_label_set_text(label, text);
    lv_obj_set_pos(label, x, y);
    lv_obj_set_size(label, width, height);

    if(style != nullptr)
        lv_label_set_style(label, style);
    else
        lv_label_set_style(label, &text_white_style);

    return label;
}