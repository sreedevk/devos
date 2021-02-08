#include <stdio.h>
#include "gfx.h"
#include "ssd1306.h"
#include "u8g2.h"
#include <math.h>
#include "storage.h"
#include <string.h>

void load_splash(u8g2_t *u8g2){
  u8g2_SetFont(u8g2, u8g2_font_10x20_tf);
  u8g2_DrawStr(u8g2, 2,17,"DEVOS");
  for(int i=0; i<100; i++) {
    u8g2_DrawBox(u8g2, 0, 26, i, 6);
    u8g2_DrawFrame(u8g2, 0, 26, 100, 6);
    u8g2_SendBuffer(u8g2);
  }
}

void load_network_info(u8g2_t *u8g2) {
  char *value = (char *) malloc(64);
  size_t ip_length = 64;
  storage_read_string("local_ip_addr", value, &ip_length);
  u8g2_SetFont(u8g2, u8g2_font_5x7_tf);
  char *ip_string = (char *) malloc(256);
  snprintf(ip_string, 256, "IP ADDR: %s\n", value);  
  u8g2_ClearBuffer(u8g2);
  u8g2_DrawStr(u8g2, 2, 30, ip_string);
  u8g2_SendBuffer(u8g2);
}
