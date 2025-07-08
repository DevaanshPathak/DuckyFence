#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

#define UART_ID uart0
#define UART_BAUDRATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define PULSE_PIN 22

char uart_buf[128];
int buf_index = 0;

int main() {
    stdio_init_all();

    uart_init(UART_ID, UART_BAUDRATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    gpio_init(PULSE_PIN);
    gpio_set_dir(PULSE_PIN, GPIO_OUT);
    gpio_put(PULSE_PIN, 0);

    while (true) {
        if (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            if (c == '\n' || c == '\r') {
                uart_buf[buf_index] = '\0';
                char* vid_ptr = strstr(uart_buf, "VID:");
                char* pid_ptr = strstr(uart_buf, "PID:");
                if (vid_ptr && pid_ptr) {
                    gpio_put(PULSE_PIN, 1);
                    sleep_ms(150);
                    gpio_put(PULSE_PIN, 0);
                }
                buf_index = 0;
            } else if (buf_index < sizeof(uart_buf) - 1) {
                uart_buf[buf_index++] = c;
            }
        }
    }
}
