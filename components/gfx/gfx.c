#include <stdio.h>
#include "gfx.h"
#include "ssd1306.h"
#include "u8g2.h"
#include <math.h>

void load_splash(u8g2_t *u8g2){
  u8g2_SetFont(u8g2, u8g2_font_10x20_tf);
  u8g2_DrawStr(u8g2, 2,17,"DEVOS");
  for(int i=0; i<100; i++) {
    u8g2_DrawBox(u8g2, 0, 26, i, 6);
    u8g2_DrawFrame(u8g2, 0, 26, 100, 6);
    u8g2_SendBuffer(u8g2);
  }
}
