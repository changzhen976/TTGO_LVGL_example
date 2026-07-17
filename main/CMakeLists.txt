idf_component_register(SRCS "TTGO_LVGL_example_main.c" "ui.c"
                       INCLUDE_DIRS "."
                       REQUIRES nvs_flash esp_lcd esp_driver_gpio esp_driver_spi lvgl lvgl_setup indev_port network MultiButton)
