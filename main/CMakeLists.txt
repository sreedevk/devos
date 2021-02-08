idf_component_register(SRCS "espruby.c" "systasks.c"
                    INCLUDE_DIRS "include"
                    REQUIRES storage network ssd1306 u8g2 gfx audio server
                    )
