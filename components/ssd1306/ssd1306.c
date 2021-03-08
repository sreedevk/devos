#include <stdio.h>
#include <string.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "u8g2.h"
#include "ssd1306.h"

static const char *TAG = "[U8G2 GRAPHICS] ";
static const unsigned int I2C_TIMEOUT_MS = 1000;
static i2c_cmd_handle_t    handle_i2c;      // I2C handle.
static u8g2_esp32_hal_t    u8g2_esp32_hal;  // HAL state data.

void u8g2_esp32_hal_init(u8g2_esp32_hal_t u8g2_esp32_hal_param) {
	u8g2_esp32_hal = u8g2_esp32_hal_param;
}

uint8_t u8g2_esp32_i2c_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
	ESP_LOGD(TAG, "i2c_cb: Received a msg: %d, arg_int: %d, arg_ptr: %p", msg, arg_int, arg_ptr);

	switch(msg) {
		case U8X8_MSG_BYTE_SET_DC: {
			if (u8g2_esp32_hal.dc != U8G2_ESP32_HAL_UNDEFINED) {
				gpio_set_level(u8g2_esp32_hal.dc, arg_int);
			}
			break;
		}

		case U8X8_MSG_BYTE_INIT: {
			if (u8g2_esp32_hal.sda == U8G2_ESP32_HAL_UNDEFINED ||
					u8g2_esp32_hal.scl == U8G2_ESP32_HAL_UNDEFINED) {
				break;
			}

		    i2c_config_t conf;
		    conf.mode = I2C_MODE_MASTER;
		    conf.sda_io_num = u8g2_esp32_hal.sda;
		    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
		    conf.scl_io_num = u8g2_esp32_hal.scl;
		    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
		    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
        conf.clk_flags = 0;

		    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
		    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0));
			break;
		}

		case U8X8_MSG_BYTE_SEND: {
			uint8_t* data_ptr = (uint8_t*)arg_ptr;
			ESP_LOG_BUFFER_HEXDUMP(TAG, data_ptr, arg_int, ESP_LOG_VERBOSE);

			while( arg_int > 0 ) {
			   ESP_ERROR_CHECK(i2c_master_write_byte(handle_i2c, *data_ptr, ACK_CHECK_EN));
			   data_ptr++;
			   arg_int--;
			}
			break;
		}

		case U8X8_MSG_BYTE_START_TRANSFER: {
			uint8_t i2c_address = u8x8_GetI2CAddress(u8x8);
			handle_i2c = i2c_cmd_link_create();
			ESP_LOGD(TAG, "Start I2C transfer to %02X.", i2c_address>>1);
			ESP_ERROR_CHECK(i2c_master_start(handle_i2c));
			ESP_ERROR_CHECK(i2c_master_write_byte(handle_i2c, i2c_address | I2C_MASTER_WRITE, ACK_CHECK_EN));
			break;
		}

		case U8X8_MSG_BYTE_END_TRANSFER: {
			ESP_LOGD(TAG, "End I2C transfer.");
			ESP_ERROR_CHECK(i2c_master_stop(handle_i2c));
			ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_MASTER_NUM, handle_i2c, I2C_TIMEOUT_MS / portTICK_RATE_MS));
			i2c_cmd_link_delete(handle_i2c);
			break;
		}
	}
	return 0;
} // u8g2_esp32_i2c_byte_cb

uint8_t u8g2_esp32_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
	ESP_LOGD(TAG, "gpio_and_delay_cb: Received a msg: %d, arg_int: %d, arg_ptr: %p", msg, arg_int, arg_ptr);

	switch(msg) {
	// Initialize the GPIO and DELAY HAL functions.  If the pins for DC and RESET have been
	// specified then we define those pins as GPIO outputs.
		case U8X8_MSG_GPIO_AND_DELAY_INIT: {
			uint64_t bitmask = 0;
			if (u8g2_esp32_hal.dc != U8G2_ESP32_HAL_UNDEFINED) {
				bitmask = bitmask | (1ull<<u8g2_esp32_hal.dc);
			}
			if (u8g2_esp32_hal.reset != U8G2_ESP32_HAL_UNDEFINED) {
				bitmask = bitmask | (1ull<<u8g2_esp32_hal.reset);
			}
			if (u8g2_esp32_hal.cs != U8G2_ESP32_HAL_UNDEFINED) {
				bitmask = bitmask | (1ull<<u8g2_esp32_hal.cs);
			}

            if (bitmask==0) {
            	break;
            }
			gpio_config_t gpioConfig;
			gpioConfig.pin_bit_mask = bitmask;
			gpioConfig.mode         = GPIO_MODE_OUTPUT;
			gpioConfig.pull_up_en   = GPIO_PULLUP_DISABLE;
			gpioConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
			gpioConfig.intr_type    = GPIO_INTR_DISABLE;
			gpio_config(&gpioConfig);
			break;
		}

	// Set the GPIO reset pin to the value passed in through arg_int.
		case U8X8_MSG_GPIO_RESET:
			if (u8g2_esp32_hal.reset != U8G2_ESP32_HAL_UNDEFINED) {
				gpio_set_level(u8g2_esp32_hal.reset, arg_int);
			}
			break;
	// Set the GPIO client select pin to the value passed in through arg_int.
		case U8X8_MSG_GPIO_CS:
			if (u8g2_esp32_hal.cs != U8G2_ESP32_HAL_UNDEFINED) {
				gpio_set_level(u8g2_esp32_hal.cs, arg_int);
			}
			break;
	// Set the Software I²C pin to the value passed in through arg_int.
		case U8X8_MSG_GPIO_I2C_CLOCK:
			if (u8g2_esp32_hal.scl != U8G2_ESP32_HAL_UNDEFINED) {
				gpio_set_level(u8g2_esp32_hal.scl, arg_int);
//				printf("%c",(arg_int==1?'C':'c'));
			}
			break;
	// Set the Software I²C pin to the value passed in through arg_int.
		case U8X8_MSG_GPIO_I2C_DATA:
			if (u8g2_esp32_hal.sda != U8G2_ESP32_HAL_UNDEFINED) {
				gpio_set_level(u8g2_esp32_hal.sda, arg_int);
//				printf("%c",(arg_int==1?'D':'d'));
			}
			break;

	// Delay for the number of milliseconds passed in through arg_int.
		case U8X8_MSG_DELAY_MILLI:
			vTaskDelay(arg_int/portTICK_PERIOD_MS);
			break;
	}
	return 0;
} // u8g2_esp32_gpio_and_delay_cb

void initialize_display() {
  u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
  u8g2_esp32_hal.sda  = PIN_SDA;
  u8g2_esp32_hal.scl  = PIN_SCL;
  u8g2_esp32_hal_init(u8g2_esp32_hal);
}


void initialize_u8g2(u8g2_t *display_handler){
  u8g2_Setup_sh1106_i2c_128x64_noname_f(display_handler, U8G2_R0, u8g2_esp32_i2c_byte_cb, u8g2_esp32_gpio_and_delay_cb);
	u8x8_SetI2CAddress(&(display_handler->u8x8),0x78);
  u8g2_InitDisplay(display_handler);
  u8g2_SetPowerSave(display_handler, 0);
  u8g2_ClearBuffer(display_handler);
}
