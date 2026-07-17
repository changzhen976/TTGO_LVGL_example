#include "ui.h"
#include "lvgl.h"

static lv_obj_t *screen;
static lv_obj_t *btn1;
static lv_obj_t *btn2;
static lv_obj_t *slider;

void ui_init(void)
{
    screen = lv_obj_create(NULL);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    btn1 = lv_button_create(screen);
    lv_obj_set_size(btn1, 100, 30);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -84);
    lv_obj_clear_flag(btn1, LV_OBJ_FLAG_SCROLLABLE);

    btn2 = lv_button_create(screen);
    lv_obj_set_size(btn2, 100, 27);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, -30);
    lv_obj_clear_flag(btn2, LV_OBJ_FLAG_SCROLLABLE);

    slider = lv_slider_create(screen);
    lv_obj_set_size(slider, 74, 14);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, 39);

    lv_screen_load(screen);
}
