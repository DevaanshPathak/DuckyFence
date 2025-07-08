cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(duckyfence_fw)

pico_sdk_init()

add_executable(blink blink.c)
target_link_libraries(blink pico_stdlib)
pico_add_extra_outputs(blink)

add_executable(uart_logger uart_logger.c)
target_link_libraries(uart_logger pico_stdlib)
pico_add_extra_outputs(uart_logger)

add_executable(usb_detect usb_detect.c)
target_link_libraries(usb_detect pico_stdlib)
pico_add_extra_outputs(usb_detect)
