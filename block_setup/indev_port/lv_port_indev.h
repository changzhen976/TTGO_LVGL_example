/*
 *
 * @file:  lv_port_indev.h
 *
 */

/*Copy this file as "lv_port_indev.h" and set this value to "1" to enable content*/
#if 1

#ifndef LV_PORT_INDEV_TEMPL_H
#define LV_PORT_INDEV_TEMPL_H

#include "driver/gpio.h"
#include "esp_log.h"

#include "multi_button.h"


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/
#ifdef CONFIG_EXAMPLE_LILIGO_T_QT
#define BUTTON_RIGHT_GPIO GPIO_NUM_47
#define BUTTON_LEFT_GPIO GPIO_NUM_0
#endif

#ifdef CONFIG_EXAMPLE_LILIGO_T_DISPLAY
#define BUTTON_RIGHT_GPIO GPIO_NUM_35
#define BUTTON_LEFT_GPIO GPIO_NUM_0
#endif


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
extern lv_indev_t * indev_keypad;

void lv_port_indev_init(void);

void Multi_btn_timer_5ms();

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORT_INDEV_TEMPL_H*/

#endif /*Disable/Enable content*/
