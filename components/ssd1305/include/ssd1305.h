#pragma once

#define HSPI_MOSI         13
#define HSPI_MISO         19
#define HSPI_CLOCK        18
#define HSPI_CHIP_SELECT  15
#define HSPI_DC           26
#define HSPI_RESET        27

#include "u8g2.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include <stdint.h>

#define U8G2_SSD1305_HAL_UNDEFINED (-1)
#define U8G2_SSD1305_HAL_DEFAULT {U8G2_SSD1305_HAL_UNDEFINED, U8G2_SSD1305_HAL_UNDEFINED, U8G2_SSD1305_HAL_UNDEFINED, U8G2_SSD1305_HAL_UNDEFINED, U8G2_SSD1305_HAL_UNDEFINED, U8G2_SSD1305_HAL_UNDEFINED, U8G2_SSD1305_HAL_UNDEFINED }

typedef struct {
	gpio_num_t clk;
	gpio_num_t mosi;
	gpio_num_t sda; // data for I²C
	gpio_num_t scl; // clock for I²C
	gpio_num_t cs;
	gpio_num_t reset;
	gpio_num_t dc;
} u8g2_ssd1305_hal_t ;

void u8g2_ssd1305_hal_init(u8g2_ssd1305_hal_t u8g2_ssd1305_hal_param);
uint8_t u8g2_ssd1305_spi_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8g2_ssd1305_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void task_test_SSD1305(void *ignore);
