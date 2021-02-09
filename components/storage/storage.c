#include "storage.h"
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "nvs.h"
#include <stddef.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_spiffs.h"

static const char *TAG = "[STORAGE] ";

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


void init_filesystem() {
  esp_vfs_spiffs_conf_t conf = {
    .base_path = "/root",
    .partition_label = NULL,
    .max_files = 5,
    .format_if_mount_failed = true
  };

  esp_err_t ret = esp_vfs_spiffs_register(&conf);
  ESP_ERROR_CHECK(ret);
  size_t total = 0, used = 0;
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
  } else {
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
  }
}

void unmount_filesystem() {
  esp_vfs_spiffs_conf_t conf = {
    .base_path = "/root",
    .partition_label = NULL,
    .max_files = 5,
    .format_if_mount_failed = true
  };
  esp_vfs_spiffs_unregister(conf.partition_label);
}
