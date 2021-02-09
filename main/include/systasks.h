#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "u8g2.h"

void task_wifi_connect(void *ignore);
void task_audio_mapping(u8g2_t *u8g2);
