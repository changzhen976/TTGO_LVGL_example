idf_component_register(SRCS "lvgl_demo_ui.c" "lvglSpiCfg.c"
                    INCLUDE_DIRS "." 
                    REQUIRES driver lvgl esp_timer esp_lcd indev_port)
