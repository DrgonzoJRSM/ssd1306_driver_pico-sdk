#include "ssd1306.h"

static void send_command(ssd1306_t* display, uint8_t command) {
	//	I2C-пакет, состоит из контрольного байта (buf[0]) и команды (buf[1]).
	//	Значение контрольного байта 0x00 указывает на то, что отправляется команда
	uint8_t buf[2] = {0x00, command};

	i2c_write_blocking(display->i2c, SSD1306_I2C_ADRESS, buf, 2, false);
}

static void send_data(ssd1306_t* display, const uint8_t* data, size_t len) {
	//	I2C пакетБ состоит из контрольного байта (buf[0]) и данных (buf[1]..).
	uint8_t buf[len + 1];

	//	Значение контрольного байта 0X40 указывает на отправку графических данных
	buf[0] = 0x40;
	memcpy(buf + 1, data, len);

	i2c_write_blocking(display->i2c, SSD1306_I2C_ADRESS, buf, len + 1, false);
}

void ssd1306_init(ssd1306_t* display, i2c_inst_t* i2c_port, uint8_t sda_pin, uint8_t scl_pin) {
	i2c_init(i2c_port, 400000);

	gpio_set_function(sda_pin, GPIO_FUNC_I2C);
	gpio_set_function(scl_pin, GPIO_FUNC_I2C);

	gpio_pull_up(sda_pin);
	gpio_pull_up(scl_pin);

	display->i2c = i2c_port;

	//	Спящий режим дисплея
	send_command(display, SSD1306_SET_DISPLAY_OFF);
	// Установка тактовой частоты ssd1306
	send_command(display, SSD1306_SET_DISPLAY_CLOCK_DIV);
	send_command(display, 0x80);
	//	Установка мультплексора
	send_command(display, SSD1306_SET_MULTIPLEX);
	send_command(display, 0x3F);
	//	Установка аппаратного смещения дисплея
	send_command(display, SSD1306_SET_DISPLAY_OFFSET);
	send_command(display, 0x00);
	//	Установка программного смещения дисплея
	send_command(display, SSD1306_SET_STARTLINE | 0x00);
	//	Установка внешнего преобразователя напряжения
	send_command(display, SSD1306_CHARGE_PUMP);
	send_command(display, 0x14);
	//	Горизантальная адрессация памяти
	send_command(display, SSD1306_MEMORY_MODE);
	send_command(display, 0x00);
	//	Адрессация столбцов справа налево
	send_command(display, SSD1306_SEGREMAP | 0x01);
	//	Сканирование COM-линий снизу вверх
	send_command(display, SSD1306_COMSCAN_DEC);
	//	Конфигурация COM-портов
	send_command(display, SSD1306_SET_COMPINS);
	send_command(display, 0x12);
	//	Контрастсность дисплея
	send_command(display, SSD1306_SET_CONTRAST);
	send_command(display, 0xCF);
	//	Установка периода предзаряда пикселей
	send_command(display, SSD1306_SET_PRECHARGE);
	send_command(display, 0xF1);
	//	Установка VCOMH (Voltage COM High)
	send_command(display, SSD1306_SET_COM_DETECT);
	send_command(display, 0x40);
	//	Отображение содержимое RAM
	send_command(display, SSD1306_DISPLAY_ALLON_RESUME);
	//	Высокий уровень сигнала - пиксель включен
	send_command(display, SSD1306_DISPLAY_NORMAL);
	//	Включение дисплея
	send_command(display, SSD1306_SET_DISPLAY_ON);

	clear_display(display);
}

void update_display(ssd1306_t* display) {

	for (uint8_t i = 0; i < SSD1306_COUNT_PAGES; i++) {

		// Устанавливается начальный адресс страницы 
		send_command(display, SSD1306_SET_PAGE_START_ADRESS + i);
		// Устанавливается начальный столбец страницы
		send_command(display, 0x00);
		send_command(display, 0x10);

		// Отправляем буффер
		send_data(display, &display->buffer[i * SSD1306_WIDTH], SSD1306_WIDTH);
	}
	
}

void clear_display(ssd1306_t* display) {
	memset(display->buffer, 0, sizeof(display->buffer));

	update_display(display);
}

void draw_pixel(ssd1306_t* display, uint8_t x_coord, uint8_t y_coord, bool on, bool update) {

	if (x_coord >= SSD1306_WIDTH || y_coord >= SSD1306_HEIGHT) {
		return;
	}

	// Устаналивается страница
	uint8_t page = y_coord >> 3;
	// Устаналивается индекс в стобце
	uint32_t index = x_coord + page * SSD1306_WIDTH;
	// Получаем остаток от деления на 8 и на него сдвигаем единицу
	uint8_t bit_mask = 1 << (y_coord & 0x07);	

	if (on) {
		// Если PAINT_OVER, закрасим пиксель
		display->buffer[index] |= bit_mask;

	} else {
		// Иначе выключим
		display->buffer[index] &= ~bit_mask;

	}

	if (update) {
		send_command(display, SSD1306_SET_PAGE_START_ADRESS + page);
		// SSD1306 требует разделения координаты Х на полубайты
		send_command(display, x_coord & 0x0F);
		send_command(display, (x_coord >> 4) | 0x10);

		send_data(display, &display->buffer[page * SSD1306_WIDTH + x_coord], 1);
	}
	
}

void draw_circle(ssd1306_t* display, uint8_t x_coord, uint8_t y_coord, uint8_t radius, bool on, bool update) {
	int x_temp = radius;
	int y_temp = 0;
	int radius_error = 1 - x_temp;

	while (x_temp >= y_temp) {
		draw_pixel(display, x_temp + x_coord, y_temp + y_coord, on, false);
		draw_pixel(display, y_temp + x_coord, x_temp + y_coord, on, false);
		draw_pixel(display, -x_temp + x_coord, y_temp + y_coord, on, false);
		draw_pixel(display, -y_temp + x_coord, x_temp + y_coord, on, false);
		draw_pixel(display, -x_temp + x_coord, -y_temp + y_coord, on, false);
		draw_pixel(display, -y_temp + x_coord, -x_temp + y_coord, on, false);
		draw_pixel(display, x_temp + x_coord, -y_temp + y_coord, on, false);
		draw_pixel(display, y_temp + x_coord, -x_temp + y_coord, on, false);

		y_temp++;

		if (radius_error < 0) {

			radius_error += 2 * y_temp + 1;

		} else {

			x_temp--;
			radius_error += 2 * (y_temp - x_temp + 1);

		}

	}

	if (update) {
		update_display(display);
	}
	
}

void draw_line(ssd1306_t* display, uint8_t x0_coord, uint8_t y0_coord, uint8_t x1_coord, uint8_t y1_coord, bool on, bool update) {
	int dx_coord = abs(x1_coord - x0_coord);
	int dy_coord = -abs(y1_coord - y0_coord);
	int sx_coord = x0_coord < x1_coord ? 1: -1;
	int sy_coord = y0_coord < y1_coord ? 1: -1;
	int error = dx_coord + dy_coord;

	while (1) {
		draw_pixel(display, x0_coord, y0_coord, on, false);

		if ((x0_coord == x1_coord) && (y0_coord == y1_coord))  {
			break;
		}	

		int error_2 = 2 * error;

		if (error_2 >= dy_coord) {
			if (x0_coord == x1_coord) {
				break;
			}

			error += dy_coord;
			x0_coord += sx_coord;
		}

		if (error_2 <= dx_coord) {
			if (y0_coord == y1_coord) {
				break;
			}

			error += dx_coord;
			y0_coord += sy_coord;
		}
		
	}

	if (update) {
		update_display(display);
	}
	
}

#ifndef NO_FONT_H
	void draw_char(ssd1306_t* display, uint8_t x_coord, uint8_t y_coord, char symbol, bool on, bool update) {

		if (symbol < 32 || symbol > 126) {
			return;
		}

		const uint8_t* char_data = font[symbol - 32];

		for (uint8_t col = 0; col < 5; col++) {

			uint8_t col_data = char_data[col];

			for (uint8_t row = 0; row < 8; row++) {

				if (col_data & (1 << row)) {
					draw_pixel(display, x_coord + col, y_coord + row, on, UPDATE_OFF);
				}

			}

		}

		if (update) {
			update_display(display);
		}

	}

	void draw_string(ssd1306_t* display, uint8_t x_coord, uint8_t y_coord, const char* string, bool on, bool update) {
		uint8_t cursor = x_coord;

		while (*string) {
			draw_char(display, cursor, y_coord, *string, on, UPDATE_OFF);

			cursor += 6;
			string++;
		}

		if (update) {
			update_display(display);
		}

	}
#endif

#ifndef NO_PHOTO_H
	void draw_picture(ssd1306_t* display, const uint8_t* data) {
		memcpy(display->buffer, data, SSD1306_WIDTH * SSD1306_COUNT_PAGES);

		update_display(display);
	}
#endif
