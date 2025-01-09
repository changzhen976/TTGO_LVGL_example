#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"

#include "lv_port_indev.h"

#ifdef CONFIG_EXAMPLE_LILIGO_T_QT
#include "esp_lcd_gc9a01.h"
#endif



extern void example_demo_ui();

// Using SPI2 in the example
#define LCD_HOST SPI2_HOST

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T-QT parameters
#ifdef CONFIG_EXAMPLE_LILIGO_T_QT

#define EXAMPLE_LCD_PIXEL_CLOCK_HZ (20 * 1000 * 1000)
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL 0
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL
#define EXAMPLE_PIN_NUM_SCLK 3
#define EXAMPLE_PIN_NUM_MOSI 2
#define EXAMPLE_PIN_NUM_MISO -1
#define EXAMPLE_PIN_NUM_LCD_DC 6
#define EXAMPLE_PIN_NUM_LCD_RST 1
#define EXAMPLE_PIN_NUM_LCD_CS 5
#define EXAMPLE_PIN_NUM_BK_LIGHT 10
#define EXAMPLE_PIN_NUM_TOUCH_CS 15

// The pixel number in horizontal and vertical
#define EXAMPLE_LCD_H_RES 128 
#define EXAMPLE_LCD_V_RES 128 

//LCD GAP
#define EXAMPLE_LCD_GAP_X 2
#define EXAMPLE_LCD_GAP_Y 1

// swap & mirror
#define EXAMPLE_LCD_SWAP false
#define EXAMPLE_LCD_MIRROR_X true
#define EXAMPLE_LCD_MIRROR_Y true

#endif

#ifdef CONFIG_EXAMPLE_LILIGO_T_DISPLAY

#define EXAMPLE_LCD_PIXEL_CLOCK_HZ (20 * 1000 * 1000)
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL 1
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL
#define EXAMPLE_PIN_NUM_SCLK 18
#define EXAMPLE_PIN_NUM_MOSI 19
#define EXAMPLE_PIN_NUM_MISO 21
#define EXAMPLE_PIN_NUM_LCD_DC 16
#define EXAMPLE_PIN_NUM_LCD_RST 23
#define EXAMPLE_PIN_NUM_LCD_CS 5
#define EXAMPLE_PIN_NUM_BK_LIGHT 4
#define EXAMPLE_PIN_NUM_TOUCH_CS 15

// The pixel number in horizontal and vertical
#define EXAMPLE_LCD_H_RES 136
#define EXAMPLE_LCD_V_RES 240

//LCD GAP
#define EXAMPLE_LCD_GAP_X 52
#define EXAMPLE_LCD_GAP_Y 40

// swap & mirror
#define EXAMPLE_LCD_SWAP false
#define EXAMPLE_LCD_MIRROR_X false
#define EXAMPLE_LCD_MIRROR_Y false

#endif



// Bit number used to represent command and parameter
#define EXAMPLE_LCD_CMD_BITS 8
#define EXAMPLE_LCD_PARAM_BITS 8

#define EXAMPLE_LVGL_TICK_PERIOD_MS 2
#define EXAMPLE_LVGL_TASK_MAX_DELAY_MS 500
#define EXAMPLE_LVGL_TASK_MIN_DELAY_MS 1
#define EXAMPLE_LVGL_TASK_STACK_SIZE (4 * 1024)
#define EXAMPLE_LVGL_TASK_PRIORITY 2




void LVGL_SPI_initFunc();
void LVGL_SPI_Display_Widget(uint8_t index);