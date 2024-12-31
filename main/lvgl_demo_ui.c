#include "esp_log.h"
#include "lvgl.h"
#include "lv_port_indev.h"


static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    lv_obj_t * btn = lv_event_get_target(e);

    if(code == LV_EVENT_KEY) {
        ESP_LOGI("UI", "KEY EVENT!!"); 
    }

    if(code == LV_EVENT_CLICKED) {

        ESP_LOGI("UI", "CLICKED EVENT!!"); 
        static uint8_t cnt = 0;
        cnt++;

        ESP_LOGI("UI", "Current Cnt : %d", cnt); 

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

/**
 * Create a button with a label and react on click event.
 */
//example_lvgl_demo_ui
void example_demo_ui(lv_disp_t *disp)
{
    lv_group_t *group = lv_group_create();
    lv_group_set_default(group);
    lv_indev_set_group(indev_keypad, group);
    // lv_group_focus_obj(group);
    // lv_group_set_editing(group, true);

    lv_obj_t * btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_group_add_obj(group, btn);

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/
    lv_obj_center(label);
}



