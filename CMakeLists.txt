cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)

project(alarm_clock C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

if (DEFINED ENV{PICO_EXAMPLES_PATH} AND NOT PICO_EXAMPLES_PATH)
    set(PICO_EXAMPLES_PATH $ENV{PICO_EXAMPLES_PATH})
    message("Using PICO_EXAMPLES_PATH from environment ('${PICO_EXAMPLES_PATH}')")
endif()

# Initialize the SDK
pico_sdk_init()

include(example_auto_set_url.cmake)

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_executable(${PROJECT_NAME}
        main.cpp
        statemachine.cpp
        menu.cpp
        menuitem.cpp
        lib/cilo72/hw/pio.cpp
        lib/cilo72/hw/blink_forever.cpp
        lib/cilo72/hw/repeating_timer.cpp
        lib/cilo72/hw/i2c_bus.cpp
        lib/cilo72/hw/spi_bus.cpp
        lib/cilo72/hw/spi_device.cpp
        lib/cilo72/hw/uart.cpp
        lib/cilo72/hw/gpiokey.cpp
        lib/cilo72/ic/ws2812.cpp
        lib/cilo72/ic/ky_040.cpp
        lib/cilo72/ic/ssd1306.cpp
        lib/cilo72/ic/tmc5160.cpp
        lib/cilo72/ic/bh1750fvi.cpp
        lib/cilo72/ic/sd2405.cpp
        lib/cilo72/ic/df_player_pro.cpp
        lib/cilo72/motion/tmc5160.cpp
        lib/cilo72/fonts/font_8x5.cpp
        lib/cilo72/fonts/font_acme.cpp
        lib/cilo72/fonts/font_bmspa.cpp
        lib/cilo72/fonts/font_bubblesstandard.cpp
        lib/cilo72/fonts/font_crackers.cpp
        )

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_generate_pio_header(${PROJECT_NAME} ${PICO_EXAMPLES_PATH}/pio/pio_blink/blink.pio)
pico_generate_pio_header(${PROJECT_NAME} ${PICO_EXAMPLES_PATH}/pio/ws2812/ws2812.pio)

target_include_directories(${PROJECT_NAME} PUBLIC lib)

target_link_libraries(${PROJECT_NAME} PRIVATE pico_stdlib hardware_pio)
target_link_libraries(${PROJECT_NAME} PRIVATE pico_stdlib hardware_i2c)
target_link_libraries(${PROJECT_NAME} PRIVATE pico_stdlib hardware_spi)

pico_add_extra_outputs(${PROJECT_NAME})
