idf_component_register(SRCS "wifiCfg.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_wifi nvs_flash)
