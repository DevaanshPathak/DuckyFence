#include "pico/stdlib.h"

#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define BAUD_RATE 115200

int main() {
    stdio_init_all();
    
    uart_init(uart0, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    while (true) {
        printf("[DuckyFence] UART Logger Active...\n");
        sleep_ms(1000);
    }
}
