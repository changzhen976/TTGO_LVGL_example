idf_component_register(SRCS "TTGO_LVGL_example_main.c" "ui.c"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES nvs_flash lvgl lvgl_setup indev_port network)
