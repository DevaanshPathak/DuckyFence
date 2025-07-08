#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"

#define UART_ID uart0
#define UART_BAUDRATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

int main() {
    board_init();
    stdio_init_all();

    uart_init(UART_ID, UART_BAUDRATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    tusb_init();

    while (true) {
        tud_task();
        while (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            if (tud_cdc_connected()) {
                tud_cdc_write_char(c);
                tud_cdc_write_flush();
            }
        }
    }
}
