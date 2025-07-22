#ifndef COMMANDS_H
#define COMMANDS_H

//Все нужные команды для работы с ssd1306

// Адрес I2C по умолчанию
#define SSD1306_I2C_ADRESS 0x3C

//	Включение/выключие дисплея (OFF - спящий режим)	
#define SSD1306_SET_DISPLAY_OFF 0xAE
#define SSD1306_SET_DISPLAY_ON 0xAF

//	Установка тактовой частоты ssd1306 (отправить байт, который:
// 		[0:3] - частота осциллятора.
//		[7:4] - делитель частоты.)
#define SSD1306_SET_DISPLAY_CLOCK_DIV 0xD5

//	Установка мультплексора (отправить значение:
//		Для 128x64 - 0x3F
//		Для 128x32 - 0x1F)
#define SSD1306_SET_MULTIPLEX 0xA8

//	Устанавливает вертикальное (аппаратное) смещение дисплея (отправить смещение (0 - 63))
#define SSD1306_SET_DISPLAY_OFFSET 0xD3

//	Устанавливает вертикальное (программное) смещение дисплея (отправить смещение (0 - 63))
#define SSD1306_SET_STARTLINE 0x40

//	Управление встроенным проебразователем напряжения
//	(0x14 - on (EXTERNAL_VCC) / 0x10 - off (SWITCHAPP_VCC))
#define SSD1306_CHARGE_PUMP 0x8D

// Флаги испльзования внешнего питания VCC и втроенного преобразования напряжения
#define SSD1306_EXTERNAL_VCC 0x1
#define SSD1306_SWITCHAPP_VCC 0x2

//	 Установка режима адресации памяти (отправить значение:
//		0x00 - Горизонтальная адресация
//		0x01 - Вертикальная адресация
//		0x02 - Постраничная адресация)
#define SSD1306_MEMORY_MODE 0x20

//	 Устновка направления адрессации столбцов (добавить знаяение:
//		SSD1306_SEGREMAP || 0x01 - столбцы идут от справа налево)
#define SSD1306_SEGREMAP 0xA0

// Установка направления сканирования COM-линий (INC - сверху вниз / DEC - снизу вверх)
#define SSD1306_COMSCAN_INC 0xC0
#define SSD1306_COMSCAN_DEC  0xC8

//	Нaстройка конфигурации COM-пинов (128x64 - 0x12 / 128x32 - 0x02)
#define SSD1306_SET_COMPINS 0xDA

//	Устанавливает контрастность дисплея (отправить значение (1-255))
#define SSD1306_SET_CONTRAST 0x81

//	Установка периода предзаряда пикселей (отправит байт, который:
// 		[0:3] - время разряда.
//		[7:4] - время предзаряда.)
#define SSD1306_SET_PRECHARGE 0xD9

//	Установка VCOMH (Voltage COM High) (отправить значение от 0x00 до 0x70)
#define SSD1306_SET_COM_DETECT 0xDB

//	Отображает содержимое RAM / включает все пиксели 
#define SSD1306_DISPLAY_ALLON_RESUME 0xA4
#define SSD1306_DISPLAY_ALLON 0xA5

//	Режимы работы дисплея (1-пиксель включен / 0-пиксель включен)
#define SSD1306_DISPLAY_NORMAL 0xA6
#define SSD1306_DISPLAY_INVERT 0xA7

//	Активирует одну из страниц дисплея (прибавить значение от 0 до 7)
#define SSD1306_SET_PAGE_START_ADRESS 0xB0

#endif
