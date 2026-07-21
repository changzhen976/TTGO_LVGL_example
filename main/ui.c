#include "ui.h"
#include "lvgl.h"
#include "lv_port_indev.h"

static lv_obj_t *screen;
static lv_obj_t *btn1;
static lv_obj_t *btn2;
static lv_obj_t *slider;
static lv_obj_t *arc;
static lv_group_t *encoder_group;

void ui_init(void)
{
    screen = lv_obj_create(NULL);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    /* arc: 显示运算结果 */
    arc = lv_arc_create(screen);
    lv_obj_set_size(arc, 56, 56);
    lv_obj_align(arc, LV_ALIGN_CENTER, 0, -80);
    lv_arc_set_range(arc, 0, 100);
    lv_arc_set_value(arc, 50);
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);

    /* slider: 编码器调节 0~100 */
    slider = lv_slider_create(screen);
    lv_obj_set_size(slider, 100, 10);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, 10);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);

    /* buttons: 示例保留, 做小 */
    btn1 = lv_button_create(screen);
    lv_obj_set_size(btn1, 50, 20);
    lv_obj_align(btn1, LV_ALIGN_CENTER, -28, 80);
    lv_obj_clear_flag(btn1, LV_OBJ_FLAG_SCROLLABLE);

    btn2 = lv_button_create(screen);
    lv_obj_set_size(btn2, 50, 20);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 28, 80);
    lv_obj_clear_flag(btn2, LV_OBJ_FLAG_SCROLLABLE);

    /* 编码器 -> slider 绑定 */
    encoder_group = lv_group_create();
    lv_group_add_obj(encoder_group, slider);
    lv_indev_set_group(indev_encoder, encoder_group);

    lv_screen_load(screen);
}

int32_t ui_get_slider_value(void)
{
    return lv_slider_get_value(slider);
}

void ui_set_arc_value(int32_t v)
{
    lv_arc_set_value(arc, v);
}
