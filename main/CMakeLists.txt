idf_component_register(SRCS "main.c" "systasks.c"
                    INCLUDE_DIRS "include"
                    REQUIRES nvs_flash storage network ssd1306 u8g2 gfx audio server ssd1305
                    )
