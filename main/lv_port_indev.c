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
	btnRight_id,
	btnLeft_id,
};

struct Button btnRight;
struct Button btnLeft;

static uint8_t btnR_Clicked = 0;
static uint8_t btnL_Clicked = 0;
static uint8_t btnR_Double_Clicked = 0;
static uint8_t btnL_Double_Clicked = 0;

uint8_t read_button_GPIO(uint8_t button_id);
void BTNR_SingleClick_Handler(void* btn);
void BTNR_DoubleClike_Handler(void* btn);
void BTNL_SingleClick_Handler(void* btn);
void BTNL_DoubleClike_Handler(void* btn);

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

    button_init(&btnRight, read_button_GPIO, 0, btnRight_id);
	button_init(&btnLeft, read_button_GPIO, 0, btnLeft_id);

    button_attach(&btnRight,     SINGLE_CLICK,       BTNR_SingleClick_Handler);
    button_attach(&btnRight,     DOUBLE_CLICK,       BTNR_DoubleClike_Handler);
    button_attach(&btnLeft,    SINGLE_CLICK,       BTNL_SingleClick_Handler);
    button_attach(&btnLeft,     DOUBLE_CLICK,       BTNR_DoubleClike_Handler);

    button_start(&btnRight);
	button_start(&btnLeft);
}


uint8_t read_button_GPIO(uint8_t button_id)
{
	// you can share the GPIO read function with multiple Buttons
	switch(button_id)
	{
		case btnRight_id:
			return gpio_get_level(GPIO_NUM_35);
		case btnLeft_id:
			return gpio_get_level(GPIO_NUM_0);
		default:
			return 0;
	}
}

void BTNR_SingleClick_Handler(void* btn){
    btnR_Clicked = 1;
}
void BTNL_SingleClick_Handler(void* btn){
    btnL_Clicked = 1;
}

void BTNR_DoubleClike_Handler(void* btn){
    btnR_Double_Clicked = 1;
}
void BTNL_DoubleClike_Handler(void* btn){
    btnL_Double_Clicked = 1;
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
    // get state from btn cb function
    if(btnL_Clicked){
        btnL_Clicked = 0;
        ESP_LOGI("Single Click", "L !!");
    }
    if(btnR_Clicked){
        btnR_Clicked = 0;
        ESP_LOGI("Single Click", "R !!");
    }
    if(btnL_Double_Clicked){
        btnL_Double_Clicked = 0;
        ESP_LOGI("Double Click", "L !!");
    }
    if(btnR_Double_Clicked){
        btnR_Double_Clicked = 0;
        ESP_LOGI("Double Click", "R !!");
    }

    // get botton event directly
    static uint8_t btnR_EventK1 = (uint8_t)NONE_PRESS;
    static uint8_t btnL_EventK1 = (uint8_t)NONE_PRESS;
    if(btnRight.event != btnR_EventK1){
        // btn right state changed.
        btnR_EventK1 = btnRight.event;
        switch (btnRight.event)
        {
        case PRESS_DOWN:
            return 3;
            break;
        default:
            // return 0;
            break;
        }
    }

    if(btnLeft.event != btnL_EventK1){
        // btn left state changed.
        btnL_EventK1 = btnLeft.event;
        switch (btnLeft.event)
        {
        case PRESS_DOWN:
            return 4;
            break;
        default:
            // return 0;
            break;
        }
    }

    return 0;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
