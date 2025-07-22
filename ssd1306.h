#ifndef SSD1306_H
#define SSD1306_H

#include "commands.h"

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include <string.h>
#include <stdlib.h>

// #define NO_PHOTO_H	// Раскомментировать, если в проекте нету photo.h
// #define NO_FONT_H 	// Раскомментировать, если в проекте нету font.h

#ifndef NO_FONT_H
	#include "font.h"
#endif

#ifndef NO_PHOTO_H
	#include "photo.h"
#endif

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

//	Макросы для заливки и очистки фигуры
#define ERASE false
#define PAINT_OVER true
//
#define UPDATE_ON true
#define UPDATE_OFF false

//	Для экономии памяти и простоты адрессации дисплей делится на 8 
//	страниц - горизонтальных полос высотой 8 пикселей 
#define SSD1306_COUNT_PAGES SSD1306_HEIGHT / 8

typedef struct {
	//	Для выбора аппаратного I2C-контроллера (i2c0 / i2c1)
	i2c_inst_t* i2c;
	//	Буфер для значений пикселей. В каждой странице значение столбца хранится 
	//		в элемента массива (нулевой бит - верхний бит в столбце, седьмой - нижний)
	uint8_t buffer[SSD1306_WIDTH * SSD1306_COUNT_PAGES];
} ssd1306_t;

//	Инициализация дисплея (отправка команд для его настройки)
//	Принимает номер порта I2C и пины для SDA и SCL 
void ssd1306_init(ssd1306_t* display, i2c_inst_t* i2c_port, uint8_t sda_pin, uint8_t scl_pin);

//	Очистка всего дисплея (очищает буфер и обновляет дислпей)
void clear_display(ssd1306_t* display);

//	Обновление содержимого дислпея.
void update_display(ssd1306_t* display);

//	Отрисовка пикселя 
//	on: true - закрашивание пикселя / false - очистка пикселя
// 	update: true - моментальное обновление экрана / false - обновление после вызова update_display 
void draw_pixel(ssd1306_t* display, uint8_t x_coord, uint8_t y_coord, bool on, bool update);

//	Отрисовка окружности по алгоритму Брезенхэма
//	on: true - закрашивание окружности / false - очистка окружности
// 	update: true - моментальное обновление экрана / false - обновление после вызова update_display 
void draw_circle(ssd1306_t* display, uint8_t x_coord, uint8_t y_coord, uint8_t radius, bool on, bool update);

//	Отрисовка линии по алгоритму Брезенхэма
//	on: true - закрашивание линии / false - очистка линии
// 	update: true - моментальное обновление экрана / false - обновление после вызова update_display 
void draw_line(ssd1306_t* display, uint8_t x0_coord, uint8_t y0_coord, uint8_t x1_coord, uint8_t y1_coord, bool on, bool update);

#ifndef NO_FONT_H
	//	Отрисовка символа (массив font, шрифт 8x8)
	//	on: true - закрашивание символа / false - очистка символа
	// 	update: true - моментальное обновление экрана / false - обновление после вызова update_display 
	void draw_char(ssd1306_t* display, uint8_t x_coord, uint8_t y_coord, char symbol, bool on, bool update);

	//	Отрисовка строки (массив font, шрифт 5x8)
	//	on: true - закрашивание символа / false - очистка символа
	// 	update: true - моментальное обновление экрана / false - обновление после вызова update_display
	void draw_string(ssd1306_t* display, uint8_t x_coord, uint8_t y_coord, const char* string, bool on, bool update);
#endif

#ifndef NO_PHOTO_H
	// Отрисовка bitmap (моментальное обновление дисплея)
	void draw_picture(ssd1306_t* display, const uint8_t* data);
#endif

#endif
