#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

#define I2C_PORT_1 i2c1
#define I2C_SDA_PIN_1 14
#define I2C_SCL_PIN_1 15

#define I2C_PORT_2 i2c0
#define I2C_SDA_PIN_2 0
#define I2C_SCL_PIN_2 1

int main() {
	stdio_init_all();

	ssd1306_t display_1;
	ssd1306_t display_2;

	ssd1306_init(&display_1, I2C_PORT_1, I2C_SDA_PIN_1, I2C_SCL_PIN_1);
	ssd1306_init(&display_2, I2C_PORT_2, I2C_SDA_PIN_2, I2C_SCL_PIN_2);

	draw_string(&display_1, 0, 20, "Hello there", PAINT_OVER, UPDATE_ON);
	draw_string(&display_1, 0, 20, "General Kenobi", PAINT_OVER, UPDATE_ON);

	draw_picture(&display_2, bitmap_image);

	while (1) {

		tight_loop_contents();

	}

	return 0;
}
