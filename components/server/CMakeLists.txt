idf_component_register(SRCS "server.c"
  INCLUDE_DIRS "include"
  REQUIRES esp_http_server storage u8g2 ssd1306 gfx
  )
