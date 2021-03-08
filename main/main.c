#include <stdio.h>
#include <stdint.h>
#include "esp_log.h"
#include "storage.h"
#include "network.h"
#include "ssd1306.h"
#include "ssd1305.h"
#include "u8g2.h"
#include "gfx.h"
#include "systasks.h"
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "audio.h"
#include "server.h"
#include <esp_http_server.h>
#include "ssd1305.h"

char *tag = "[ESP32DEV] ";
u8g2_t u8g2;

void app_main(void) {
  u8g2_t *u8g2_pointer = (u8g2_t *) malloc(sizeof(u8g2_t));
  u8g2 = *u8g2_pointer;

  initialize_storage();
  initialize_display();
  initialize_u8g2(&u8g2);


  //xTaskCreate(&task_wifi_connect, "task_wifi_connect", 4096, NULL, 5, NULL);
  //start_webserver(); 
  //load_splash(&u8g2);
  load_network_info(&u8g2);
  xTaskCreate(&task_draw_sine, "draw_sine_task", 2048, &u8g2, 5, NULL);


  //load_bitmap(&u8g2);

  //task_audio_mapping(&u8g2);
  fflush(stdout);
}
