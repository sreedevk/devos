#include <stdio.h>
#include <stdbool.h>
#include "audio.h"
#include <stdint.h>
#include "driver/adc.h"
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "ssd1306.h"
#include "u8g2.h"
#include <math.h>
#include "storage.h"
#include "esp_log.h"

static const char *tag = "Audio GFX";

void init_audio_mapping(u8g2_t *u8g2){
  int sample;
  int xpix = 0;
  int ypix = 20;
  int prevx = 0;
  int prevy = 0;
  int32_t audio_sampling_stop;

  while(true) {
    vTaskDelay(100/portTICK_RATE_MS);
    storage_read("sample_audio", &audio_sampling_stop);
    if(audio_sampling_stop == 0) break;
    if(xpix == 127) {
      u8g2_SetDrawColor(u8g2, 0);
      u8g2_DrawBox(u8g2, 0, 0, 128, 22);
      u8g2_SendBuffer(u8g2);
      u8g2_SetDrawColor(u8g2, 1);
    } 
    xpix = (xpix+1) % 128;
    sample = adc1_get_raw(ADC1_CHANNEL_6);
    ypix = 20 - (((double ) sample/4096.0) * 20);
    u8g2_DrawLine(u8g2, prevx, prevy, xpix, ypix);
    u8g2_SendBuffer(u8g2);
    prevx = xpix;
    prevy = ypix;
  }
  vTaskDelete(NULL);
}

void play_note(double freq) {
  for(int i=0; i<1000; i++) {
    gpio_set_level(GPIO_NUM_18, 1);
    vTaskDelay(freq/portTICK_PERIOD_MS);
    gpio_set_level(GPIO_NUM_18, 0);
    vTaskDelay(freq/portTICK_PERIOD_MS);
  }
}

void play_notes(){
  gpio_set_direction(GPIO_NUM_18, GPIO_MODE_OUTPUT);
  while(true) {
    play_note(NOTE_A);
    vTaskDelay(100/portTICK_PERIOD_MS);
    play_note(NOTE_A);
    vTaskDelay(100/portTICK_PERIOD_MS);
    vTaskDelay(NOTE_C);
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

void task_draw_sine(void *param) {
  u8g2_t *u8g2 = (u8g2_t *) param;
  u8g2_ClearBuffer(u8g2);

  double y = 0, offset = 0;
  double amplitude = 7, frequency = 0.5;

  while(true) {
    u8g2_ClearBuffer(u8g2);
    for(double x = 0; x < 127; x++) {
      y = (amplitude * round(sin((x + offset) * frequency))) + 32;
      u8g2_DrawPixel(u8g2, x, y);
    }
    u8g2_SendBuffer(u8g2);
    offset += 1;
  }
  vTaskDelete(NULL);
}
