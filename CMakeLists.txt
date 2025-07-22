cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(ssd1306 C CXX ASM)

pico_sdk_init()

add_executable(ssd1306
    main.c
    ssd1306.c
    font.c
    photo.c
    font.c
)

pico_enable_stdio_usb(ssd1306 1)
pico_enable_stdio_uart(ssd1306 0)

pico_add_extra_outputs(ssd1306)

target_link_libraries(ssd1306
    pico_stdlib
    hardware_i2c
)
