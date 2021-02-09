idf_component_register(SRCS "audio.c"
                    INCLUDE_DIRS "include"
                    REQUIRES ssd1306 u8g2 nvs_flash storage
                    )
