#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"  // You'll need to add this from pico-extras

#define NEOPIXEL_PIN 3
#define IS_RGBW false

void set_pixel_color(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = ((uint32_t)(r) << 16) | ((uint32_t)(g) << 8) | b;
    pio_sm_put_blocking(pio0, 0, color << 8u); // Left shift for GRB 24-bit
}

int main() {
    stdio_init_all();

    // Init WS2812
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, NEOPIXEL_PIN, 800000, IS_RGBW);

    // Default: yellow = no USB
    set_pixel_color(255, 255, 0);
    sleep_ms(1000);

    while (true) {
        // Simulated detection logic
        bool usb_connected = true;   // Replace with actual TinyUSB check
        bool is_bad = false;         // Set this true if bad VID/PID

        if (!usb_connected) {
            set_pixel_color(255, 255, 0); // Yellow
        } else if (is_bad) {
            set_pixel_color(255, 0, 0);   // Red
        } else {
            set_pixel_color(0, 255, 0);   // Green
        }

        sleep_ms(500); // Update every 0.5 sec
    }
}
