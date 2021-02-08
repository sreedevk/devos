#include <stdio.h>
#include <stdint.h>
#include "esp_log.h"
#include "storage.h"
#include "network.h"
#include "ssd1306.h"
#include "u8g2.h"
#include "gfx.h"
#include "systasks.h"
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "audio.h"
#include "server.h"
#include <esp_http_server.h>

char *tag = "[ESP32DEV] ";

void app_main(void) {
  u8g2_t *u8g2_pointer = (u8g2_t *) malloc(sizeof(u8g2_t));
  u8g2_t u8g2 = *u8g2_pointer;

  initialize_display();
  initialize_u8g2(&u8g2);
  initialize_storage();
  xTaskCreate(&task_wifi_connect, "task_wifi_connect", 4096, NULL, 5, NULL);
  load_splash(&u8g2);
  httpd_handle_t server = start_webserver();
  init_audio_mapping(&u8g2);
  stop_webserver(server);
  fflush(stdout);
}
