#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

#define UART_ID uart0
#define UART_BAUDRATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define ALERT_PIN 22

char uart_buf[128];
int buf_index = 0;

typedef struct {
    uint16_t vid;
    uint16_t pid;
    const char* label;
} BadUSB;

BadUSB blacklist[] = {
    {0x16D0, 0x27DB, "Rubber Ducky"},
    {0x1781, 0x0C9F, "Malduino"},
    {0x1A86, 0x7523, "Cheap HID Clone"},
    {0x2341, 0x8036, "Uno Clone"},
};
const int blacklist_len = sizeof(blacklist) / sizeof(blacklist[0]);

void check_vid_pid(uint16_t vid, uint16_t pid) {
    for (int i = 0; i < blacklist_len; i++) {
        if (blacklist[i].vid == vid && blacklist[i].pid == pid) {
            printf("[ALERT] Blocked Device Detected! %s (VID: %04X, PID: %04X)\n", blacklist[i].label, vid, pid);
            gpio_put(ALERT_PIN, 1);
            return;
        }
    }
    printf("[INFO] Safe device: VID: %04X, PID: %04X\n", vid, pid);
    gpio_put(ALERT_PIN, 0);
}

int main() {
    stdio_init_all();

    uart_init(UART_ID, UART_BAUDRATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    gpio_init(ALERT_PIN);
    gpio_set_dir(ALERT_PIN, GPIO_OUT);
    gpio_put(ALERT_PIN, 0);

    printf("usb_filter: Waiting for USB device logs via UART...\n");

    while (true) {
        if (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            if (c == '\n' || c == '\r') {
                uart_buf[buf_index] = '\0';
                char* vid_ptr = strstr(uart_buf, "VID:");
                char* pid_ptr = strstr(uart_buf, "PID:");
                if (vid_ptr && pid_ptr) {
                    uint16_t vid = 0, pid = 0;
                    sscanf(vid_ptr + 4, "%hx", &vid);
                    sscanf(pid_ptr + 4, "%hx", &pid);
                    check_vid_pid(vid, pid);
                }
                buf_index = 0;
            } else if (buf_index < sizeof(uart_buf) - 1) {
                uart_buf[buf_index++] = c;
            }
        }
    }
}
