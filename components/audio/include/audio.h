#pragma once
#include "u8g2.h"


#define NOTE_A 27.50
#define NOTE_B 30.87
#define NOTE_C 16.35
#define NOTE_D 18.35
#define NOTE_E 20.60
#define NOTE_F 21.83
#define NOTE_G 24.50

void init_audio_mapping(u8g2_t *u8g2);
void play_notes();
void task_draw_sine(void *param);
