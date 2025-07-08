#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"

void tud_mount_cb(void) {
    printf("[USB] Device mounted.\n");
}

void tud_umount_cb(void) {
    printf("[USB] Device unmounted.\n");
}

void tud_suspend_cb(bool remote_wakeup_en) {
    printf("[USB] Device suspended.\n");
}

void tud_resume_cb(void) {
    printf("[USB] Device resumed.\n");
}

int main(void) {
    board_init();
    stdio_init_all();
    tusb_init();

    printf("DuckyFence USB detect firmware started.\n");

    while (1) {
        tud_task();

    }
}
