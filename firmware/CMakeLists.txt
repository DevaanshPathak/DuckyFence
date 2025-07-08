cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(duckyfence_fw)

pico_sdk_init()

# -------------------
# Standard Executables
# -------------------

add_executable(blink blink.c)
target_link_libraries(blink pico_stdlib)
pico_add_extra_outputs(blink)

add_executable(uart_logger uart_logger.c)
target_link_libraries(uart_logger pico_stdlib)
pico_add_extra_outputs(uart_logger)

add_executable(arduino_uart_logger arduino_uart_logger.c)
target_link_libraries(arduino_uart_logger pico_stdlib)
pico_add_extra_outputs(arduino_uart_logger)

add_executable(gpio_pulse gpio_pulse.c)
target_link_libraries(gpio_pulse pico_stdlib)
pico_add_extra_outputs(gpio_pulse)

add_executable(usb_filter usb_filter.c)
target_link_libraries(usb_filter pico_stdlib)
pico_add_extra_outputs(usb_filter)

add_executable(temp temp.c)
target_link_libraries(temp pico_stdlib)
pico_add_extra_outputs(temp)

# -------------------
# TinyUSB-Based Executables
# -------------------

add_executable(usb_detect usb_detect.c)
target_link_libraries(usb_detect pico_stdlib tinyusb_device tinyusb_board)
pico_add_extra_outputs(usb_detect)

add_executable(usb_passthrough usb_passthrough.c)
target_link_libraries(usb_passthrough pico_stdlib tinyusb_device tinyusb_board)
pico_add_extra_outputs(usb_passthrough)
