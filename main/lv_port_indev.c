/**
 * @file lv_port_indev_templ.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev.h"
#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/

/* Multi Button */
enum Button_IDs {
	btnPlus_id,
	btnMinus_id,
};

struct Button btnPlus;
struct Button btnMinus;

static uint8_t btnP_Pressed = 0;
static uint8_t btnM_Pressed = 0;

uint8_t read_button_GPIO(uint8_t button_id);
void BTNP_PRESS_DOWN_Handler(void* btn);
void BTNP_PRESS_UP_Handler(void* btn);
void BTNM_PRESS_DOWN_Handler(void* btn);
void BTNM_PRESS_UP_Handler(void* btn);

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void keypad_init(void);
static void keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static uint32_t keypad_get_key(void);

/**********************
 *  STATIC VARIABLES
 **********************/

lv_indev_t *indev_keypad;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
    /**
     * Here you will find example implementation of input devices supported by LittelvGL:
     *  - Touchpad
     *  - Mouse (with cursor support)
     *  - Keypad (supports GUI usage only with key)
     *  - Encoder (supports GUI usage only with: left, right, push)
     *  - Button (external buttons to press points on the screen)
     *
     *  The `..._read()` function are only examples.
     *  You should shape them according to your hardware
     */

    static lv_indev_drv_t indev_drv;

    /*------------------
     * Keypad
     * -----------------*/

    /*Initialize your keypad or keyboard if you have*/
    keypad_init();

    /*Register a keypad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keypad_read;
    indev_keypad = lv_indev_drv_register(&indev_drv);

    /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     *add objects to the group with `lv_group_add_obj(group, obj)`
     *and assign this input device to group to navigate in it:
     *`lv_indev_set_group(indev_keypad, group);`*/
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Keypad
 * -----------------*/

/*Initialize your keypad*/
static void keypad_init(void)
{
    /* GPIO Init */
    gpio_reset_pin(GPIO_NUM_35);
    gpio_set_direction(GPIO_NUM_35, GPIO_MODE_INPUT);

    gpio_reset_pin(GPIO_NUM_0);
    gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
    ESP_LOGI("GPIO", "Initialize GPIO OK");

    /*---- Muilt Button Init ----*/

    button_init(&btnPlus, read_button_GPIO, 0, btnPlus_id);
	button_init(&btnMinus, read_button_GPIO, 0, btnMinus_id);

    button_attach(&btnPlus,     PRESS_DOWN,       BTNP_PRESS_DOWN_Handler);
    button_attach(&btnPlus,     PRESS_UP,       BTNP_PRESS_UP_Handler);
    button_attach(&btnMinus,    PRESS_DOWN,       BTNM_PRESS_DOWN_Handler);
    button_attach(&btnMinus,     PRESS_UP,       BTNP_PRESS_UP_Handler);

    button_start(&btnPlus);
	button_start(&btnMinus);
}


uint8_t read_button_GPIO(uint8_t button_id)
{
	// you can share the GPIO read function with multiple Buttons
	switch(button_id)
	{
		case btnPlus_id:
			return gpio_get_level(GPIO_NUM_35);
		case btnMinus_id:
			return gpio_get_level(GPIO_NUM_0);
		default:
			return 0;
	}
}

void BTNP_PRESS_DOWN_Handler(void* btn){
    btnP_Pressed = 1;
}
void BTNM_PRESS_DOWN_Handler(void* btn){
    btnM_Pressed = 1;
}

void BTNP_PRESS_UP_Handler(void* btn){
    btnP_Pressed = 0;
}
void BTNM_PRESS_UP_Handler(void* btn){
    btnM_Pressed = 0;
}

void Multi_btn_timer_5ms(){
    button_ticks();
}


/*Will be called by the library to read the mouse*/
static void keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static uint32_t last_key = 0;

    /*Get the current x and y coordinates*/
    // mouse_get_xy(&data->point.x, &data->point.y);

    /*Get whether the a key is pressed and save the pressed key*/
    uint32_t act_key = keypad_get_key();

    if (act_key != 0)
    {
        // ESP_LOGI("GPIO", "Get GPIO State ");
        data->state = LV_INDEV_STATE_PR;

        /*Translate the keys to LVGL control characters according to your key definitions*/
        switch (act_key)
        {
        case 1:
            act_key = LV_KEY_ENTER;
            break;
        case 2:
            act_key = LV_KEY_PREV;
            break;
        case 3:
            act_key = LV_KEY_LEFT;
            break;
        case 4:
            act_key = LV_KEY_RIGHT;
            break;
        case 5:
            act_key = LV_KEY_ENTER;
            break;
        }

        last_key = act_key;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    data->key = last_key;
}

/*Get the currently being pressed key.  0 if no key is pressed*/
static uint32_t keypad_get_key(void)
{
    /*Your code comes here*/
    if (btnP_Pressed)
    {
        btnP_Pressed = 0;
        return 3;
    }

    if (btnM_Pressed)
    {
        btnM_Pressed = 0;
        return 4;
    }
    return 0;
}

/*------------------
 * Encoder
 * -----------------*/

/*------------------
 * Button
 * -----------------*/

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
