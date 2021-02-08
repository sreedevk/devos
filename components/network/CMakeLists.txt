idf_component_register(SRCS "network.c"
                    INCLUDE_DIRS "include"
                    REQUIRES nvs_flash storage ssd1306 gfx u8g2
                    )
