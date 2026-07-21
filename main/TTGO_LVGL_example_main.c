/*
 * SPDX-FileCopyrightText: 2021-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "lvglSpiCfg.h"
#include "lv_port_indev.h"

#include "wifiCfg.h"
#include "ui.h"

// static const char *TAG = "example";

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    
    LVGL_SPI_initFunc();

    ui_init();

    wifi_init_sta();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
        Multi_btn_timer_5ms();
        Encoder_timer_5ms();
    }
    
}
