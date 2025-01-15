// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "../ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Button1 = lv_btn_create(ui_Screen1);
    lv_obj_set_width(ui_Button1, 100);
    lv_obj_set_height(ui_Button1, 30);
    lv_obj_set_x(ui_Button1, 0);
    lv_obj_set_y(ui_Button1, -84);
    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Button1, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Button1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Button2 = lv_btn_create(ui_Screen1);
    lv_obj_set_width(ui_Button2, 100);
    lv_obj_set_height(ui_Button2, 27);
    lv_obj_set_x(ui_Button2, 0);
    lv_obj_set_y(ui_Button2, -30);
    lv_obj_set_align(ui_Button2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Slider1 = lv_slider_create(ui_Screen1);
    lv_slider_set_value(ui_Slider1, 0, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_Slider1) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_Slider1, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_Slider1, 74);
    lv_obj_set_height(ui_Slider1, 14);
    lv_obj_set_x(ui_Slider1, 0);
    lv_obj_set_y(ui_Slider1, 39);
    lv_obj_set_align(ui_Slider1, LV_ALIGN_CENTER);



}