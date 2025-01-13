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

static void encoder_int();
static void encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
QueueHandle_t queue = NULL;
static pcnt_unit_handle_t pcnt_unit = NULL;
static int32_t encoder_diff;
static lv_indev_state_t encoder_state;

/**********************
 *  STATIC VARIABLES
 **********************/

lv_indev_t *indev_keypad;
lv_indev_t *indev_encoder;

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
    encoder_int();

    /*Register a keypad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keypad_read;
    indev_keypad = lv_indev_drv_register(&indev_drv);

    /*Register a encoder input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    indev_encoder = lv_indev_drv_register(&indev_drv);

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
    gpio_reset_pin(BUTTON_RIGHT_GPIO);
    gpio_set_direction(BUTTON_RIGHT_GPIO, GPIO_MODE_INPUT);

    gpio_reset_pin(BUTTON_LEFT_GPIO);
    gpio_set_direction(BUTTON_LEFT_GPIO, GPIO_MODE_INPUT);
    ESP_LOGI("GPIO", "Initialize GPIO OK");

    /*---- Muilt Button Init ----*/

    button_init(&btnRight, read_button_GPIO, 0, btnRight_id);
	button_init(&btnLeft, read_button_GPIO, 0, btnLeft_id);

    button_attach(&btnRight,     SINGLE_CLICK,       BTNR_SingleClick_Handler);
    button_attach(&btnRight,     DOUBLE_CLICK,       BTNR_DoubleClike_Handler);
    button_attach(&btnLeft,    SINGLE_CLICK,       BTNL_SingleClick_Handler);
    button_attach(&btnLeft,     DOUBLE_CLICK,       BTNL_DoubleClike_Handler);

    button_start(&btnRight);
	button_start(&btnLeft);
}


uint8_t read_button_GPIO(uint8_t button_id)
{
	// you can share the GPIO read function with multiple Buttons
	switch(button_id)
	{
		case btnRight_id:
			return gpio_get_level(BUTTON_RIGHT_GPIO);
		case btnLeft_id:
			return gpio_get_level(BUTTON_LEFT_GPIO);
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
            act_key = LV_KEY_NEXT;
            break;
        case 2:
            act_key = LV_KEY_PREV;
            break;
        case 3:
            act_key = LV_KEY_ENTER;
            break;
        case 4:
            act_key = LV_KEY_BACKSPACE;
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
        return 2;
    }
    if(btnR_Clicked){
        btnR_Clicked = 0;
        ESP_LOGI("Single Click", "R !!");
        return 1;
    }
    if(btnL_Double_Clicked){
        btnL_Double_Clicked = 0;
        ESP_LOGI("Double Click", "L !!");
        return 4;
    }
    if(btnR_Double_Clicked){
        btnR_Double_Clicked = 0;
        ESP_LOGI("Double Click", "R !!");
        return 5;
    }

    // // get botton event directly
    // static uint8_t btnR_EventK1 = (uint8_t)NONE_PRESS;
    // static uint8_t btnL_EventK1 = (uint8_t)NONE_PRESS;
    // if(btnRight.event != btnR_EventK1){
    //     // btn right state changed.
    //     btnR_EventK1 = btnRight.event;
    //     switch (btnRight.event)
    //     {
    //     case PRESS_DOWN:
    //         return 3;
    //         break;
    //     default:
    //         // return 0;
    //         break;
    //     }
    // }

    // if(btnLeft.event != btnL_EventK1){
    //     // btn left state changed.
    //     btnL_EventK1 = btnLeft.event;
    //     switch (btnLeft.event)
    //     {
    //     case PRESS_DOWN:
    //         return 4;
    //         break;
    //     default:
    //         // return 0;
    //         break;
    //     }
    // }

    return 0;
}


static bool example_pcnt_on_reach(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata, void *user_ctx)
{
    BaseType_t high_task_wakeup;
    QueueHandle_t queue = (QueueHandle_t)user_ctx;
    // send event data to queue, from this interrupt callback
    xQueueSendFromISR(queue, &(edata->watch_point_value), &high_task_wakeup);
    return (high_task_wakeup == pdTRUE);
}

static void encoder_int()
{
    ESP_LOGI("Encoder Init", "install pcnt unit");
    pcnt_unit_config_t unit_config = {
        .high_limit = EXAMPLE_PCNT_HIGH_LIMIT,
        .low_limit = EXAMPLE_PCNT_LOW_LIMIT,
    };
    
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

    ESP_LOGI("Encoder Init", "set glitch filter");
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 1000,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

    ESP_LOGI("Encoder Init", "install pcnt channels");
    pcnt_chan_config_t chan_a_config = {
        .edge_gpio_num = ENCODER_A_GPIO,
        .level_gpio_num = ENCODER_B_GPIO,
    };
    pcnt_channel_handle_t pcnt_chan_a = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_a_config, &pcnt_chan_a));
    pcnt_chan_config_t chan_b_config = {
        .edge_gpio_num = ENCODER_B_GPIO,
        .level_gpio_num = ENCODER_A_GPIO,
    };
    pcnt_channel_handle_t pcnt_chan_b = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_b_config, &pcnt_chan_b));

    ESP_LOGI("Encoder Init", "set edge and level actions for pcnt channels");
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    ESP_LOGI("Encoder Init", "add watch points and register callbacks");
    int watch_points[] = {EXAMPLE_PCNT_LOW_LIMIT, -50, 0, 50, EXAMPLE_PCNT_HIGH_LIMIT};
    for (size_t i = 0; i < sizeof(watch_points) / sizeof(watch_points[0]); i++) {
        ESP_ERROR_CHECK(pcnt_unit_add_watch_point(pcnt_unit, watch_points[i]));
    }
    pcnt_event_callbacks_t cbs = {
        .on_reach = example_pcnt_on_reach,
    };
    queue = xQueueCreate(10, sizeof(int));
    ESP_ERROR_CHECK(pcnt_unit_register_event_callbacks(pcnt_unit, &cbs, queue));

    ESP_LOGI("Encoder Init", "enable pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_LOGI("Encoder Init", "clear pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_LOGI("Encoder Init", "start pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));

}

static void encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    data->enc_diff = encoder_diff;
    data->state = encoder_state;

    encoder_diff = 0;
}

void Encoder_timer_5ms()
{
    static int event_count = 0;
    static int pulse_count = 0;
    static int pulse_count_K1 = 0;

    if (xQueueReceive(queue, &event_count, 0)) {
        ESP_LOGI("TAG", "Watch point event, count: %d", event_count);
    }

    ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &pulse_count));
    // ESP_LOGI("TAG", "Pulse count: %d", pulse_count);

    if(
        (pulse_count - pulse_count_K1 > EXAMPLE_ENCODER_DIFF_STEP/* Crt Value > Prev Value */ 
        && pulse_count - pulse_count_K1 < EXAMPLE_PCNT_HIGH_LIMIT / 2 )/* in case low limit overflow */ 
        ||
        (pulse_count - pulse_count_K1 > EXAMPLE_ENCODER_DIFF_STEP - EXAMPLE_PCNT_HIGH_LIMIT/* High limit overflow */ 
        && pulse_count - pulse_count_K1 < -1 * (EXAMPLE_PCNT_HIGH_LIMIT / 2 ) /* make sure high limit */) 
        ||
        pulse_count - pulse_count_K1 > EXAMPLE_ENCODER_DIFF_STEP + EXAMPLE_PCNT_HIGH_LIMIT/* in case like '-99 -> -1 -> +2 -> +4 ...' */
        )
    {
        encoder_diff++;
        pulse_count_K1 = pulse_count;
        ESP_LOGI("Encoder Plus", "Pulse count: %d", pulse_count);
    }else if(
        (pulse_count - pulse_count_K1 < -EXAMPLE_ENCODER_DIFF_STEP/* Crt Value < Prev Value */ 
        && pulse_count - pulse_count_K1 > EXAMPLE_PCNT_LOW_LIMIT / 2 /* in case high limit overflow */) 
        ||
        (pulse_count - pulse_count_K1 < -EXAMPLE_ENCODER_DIFF_STEP - EXAMPLE_PCNT_LOW_LIMIT/* low limit overflow */ 
        && pulse_count - pulse_count_K1 > -1 * (EXAMPLE_PCNT_LOW_LIMIT / 2 ) /* make sure high limit */) 
        ||
        pulse_count - pulse_count_K1 < -EXAMPLE_ENCODER_DIFF_STEP + EXAMPLE_PCNT_LOW_LIMIT/* in case like '+99 -> +1 -> -2 -> -4 ...' */
        )
    {
        encoder_diff--;
        pulse_count_K1 = pulse_count;
        ESP_LOGI("Encoder Minus", "Pulse count: %d", pulse_count);
    }

    // encoder_diff = 0;
    encoder_state = LV_INDEV_STATE_REL;
    
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
