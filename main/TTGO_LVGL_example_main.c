/*
 * SPDX-FileCopyrightText: 2021-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

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
#include "lvglSpiCfg.h"
#include "lv_port_indev.h"

#include "multi_button.h"
#include "wifiCfg.h"

// static const char *TAG = "example";

extern void example_demo_ui();

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    
    LVGL_SPI_initFunc();

    LVGL_SPI_Display_Widget(0);

    wifi_init_sta();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
        Multi_btn_timer_5ms();
        Encoder_timer_5ms();
    }
    
}
