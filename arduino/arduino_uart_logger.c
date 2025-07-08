#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>

#define UART_ID uart0
#define UART_BAUDRATE 115200
#define UART_RX_PIN 1
#define UART_TX_PIN 0

char uart_buf[128];
int buf_index = 0;

int main() {
    stdio_init_all();

    uart_init(UART_ID, UART_BAUDRATE);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);

    printf("DuckyFence RP2040 UART logger started.\n");

    while (true) {
        if (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);

            if (c == '\n' || c == '\r') {
                uart_buf[buf_index] = '\0';

                char *vid_ptr = strstr(uart_buf, "VID:");
                char *pid_ptr = strstr(uart_buf, "PID:");
                if (vid_ptr && pid_ptr) {
                    uint16_t vid = 0, pid = 0;
                    sscanf(vid_ptr + 4, "%hx", &vid);
                    sscanf(pid_ptr + 4, "%hx", &pid);
                    printf("[USB Device] VID: 0x%04X, PID: 0x%04X\n", vid, pid);
                } else {
                    printf("[UART] %s\n", uart_buf);
                }

                buf_index = 0;
            } else if (buf_index < sizeof(uart_buf) - 1) {
                uart_buf[buf_index++] = c;
            }
        }
    }
}
