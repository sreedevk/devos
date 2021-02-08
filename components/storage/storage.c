#include "storage.h"
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "nvs.h"
#include <stddef.h>

void initialize_storage() {
  esp_err_t error = nvs_flash_init();
  if (error == ESP_ERR_NVS_NO_FREE_PAGES || error == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    error = nvs_flash_init();
  }
  ESP_ERROR_CHECK(error);
}

void storage_write(const char *key, int32_t value) {
  nvs_handle_t nvs_storage_handler;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_storage_handler);
  nvs_set_i32(nvs_storage_handler, key, value);
  err = nvs_commit(nvs_storage_handler);
  ESP_ERROR_CHECK(err);
  nvs_close(nvs_storage_handler);
}

void storage_write_string(const char *key, const char *value) {
  nvs_handle_t nvs_storage_handler;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_storage_handler);
  err = nvs_set_str(nvs_storage_handler, key, value);
  ESP_ERROR_CHECK(err);
  nvs_close(nvs_storage_handler);
}

void storage_read_string(const char *key, char *value, size_t *strlength) {
  nvs_handle_t nvs_storage_handler;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_storage_handler);
  err = nvs_get_str(nvs_storage_handler, key, value, strlength);
  ESP_ERROR_CHECK(err);
  nvs_close(nvs_storage_handler);
}

void storage_read(const char *key, int32_t *value) {
  nvs_handle_t nvs_storage_handler;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_storage_handler);
  err = nvs_get_i32(nvs_storage_handler, key, value);
  ESP_ERROR_CHECK(err);
  nvs_close(nvs_storage_handler);
}

