#include "systasks.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "network.h"
#include "esp_log.h"
#include "gfx.h"
#include "u8g2.h"
#include "audio.h"
#include "storage.h"

static const char *TAG = "[WIFI CONNECT] ";

void task_wifi_connect(void *ignore) {
  ESP_LOGI(TAG, "Wifi Connect Task Running");
  initialize_wifi();
  vTaskDelete(NULL);
}

void task_audio_mapping(u8g2_t *u8g2) {
  storage_write("sample_audio", 1);
  init_audio_mapping(u8g2);
}
