idf_component_register(SRCS "lvglSpiCfg.c"
                    INCLUDE_DIRS "."
                    REQUIRES     lvgl esp_lcd_gc9a01 esp_timer
                    PRIV_REQUIRES esp_lcd indev_port esp_driver_gpio esp_driver_spi)
