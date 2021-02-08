#include <stdio.h>
#include <stdint.h>
#include "esp_log.h"
#include "storage.h"
#include "network.h"
#include "ssd1306.h"

char *tag = "[ESP32DEV] ";

void app_main(void) {
  initialize_storage();
  storage_write("example_int", (int32_t) 64);
  ESP_LOGI(tag, "VALUE HAS BEEN SET. NOW WILL TRY TO RETRIEVE VALUE");

  int32_t retval = 0;
  storage_read("example_int", &retval);
  ESP_LOGI(tag, "RETRIECED VALUE IS %d", retval);

  display_test();

  wifi_connect();
  fflush(stdout);
}
