#include "esp_log.h"
#include "lvgl.h"
#include "lv_port_indev.h"


static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    lv_obj_t * btn = lv_event_get_target(e);

    if(code == LV_EVENT_KEY) {
        ESP_LOGI("UI", "KEY EVENT Catched!!"); 
        uint32_t key = lv_event_get_key(e);
        ESP_LOGI("UI", "Get KEY : %d", (uint8_t)key);

        static uint8_t cnt = 0;

        if (key == LV_KEY_ENTER && cnt <=49){
            cnt++;
        }else if(key == LV_KEY_BACKSPACE && cnt >=1){
            cnt--;
        }

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Cnt: %d", cnt);
    }

}

static void btn1_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    lv_obj_t * btn = lv_event_get_target(e);

    if(code == LV_EVENT_KEY) {
        ESP_LOGI("UI", "KEY EVENT Catched!!"); 
        uint32_t key = lv_event_get_key(e);
        ESP_LOGI("UI", "Get KEY : %d", (uint8_t)key);

        static uint8_t cnt = 0;

        if (key == LV_KEY_ENTER && cnt <=49){
            cnt += 2;
        }else if(key == LV_KEY_BACKSPACE && cnt >=1){
            cnt -= 2;
        }

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Cnt1: %d", cnt);
    }

}

/**
 * Create a button with a label and react on click event.
 */
//example_lvgl_demo_ui
void example_demo_ui()
{
    lv_group_t *group = lv_group_create();
    lv_group_set_default(group);
    // lv_indev_set_group(indev_keypad, group);
    lv_indev_set_group(indev_encoder, group);
    // lv_group_focus_obj(group);
    // lv_group_set_editing(group, true);

    lv_obj_t * btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    // lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 55, 25);                          /*Set its size*/
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, -20);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_KEY, NULL);           /*Assign a callback to the button*/

    lv_obj_t * btn1 = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    // lv_obj_set_pos(btn1, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn1, 55, 25);                          /*Set its size*/
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 20);
    lv_obj_add_event_cb(btn1, btn1_event_cb, LV_EVENT_KEY, NULL);           /*Assign a callback to the button*/


    lv_group_add_obj(group, btn);
    lv_group_add_obj(group, btn1);

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/
    lv_obj_center(label);

    lv_obj_t * label1 = lv_label_create(btn1);          /*Add a label to the button*/
    lv_label_set_text(label1, "Button");                     /*Set the labels text*/
    lv_obj_center(label1);
}



