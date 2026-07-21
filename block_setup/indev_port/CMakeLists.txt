idf_component_register(SRCS "lv_port_indev.c"
                    INCLUDE_DIRS "."
                    REQUIRES     lvgl MultiButton esp_driver_gpio esp_driver_pcnt)
