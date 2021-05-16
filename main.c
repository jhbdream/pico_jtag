#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"

#include "bsp/board.h"
#include "tusb.h"

#include "led.h"
#include "pio_jtag.h"

int main()
{
    stdio_usb_init();
    led_blinking_init();

    printf("board init ok!\n");
    jtag_init(pio_jtag, 1000000);

    JTAG_TAP_SHIFT_IR;

    sleep_ms(200);
    uint32_t write = 0x5a;
    uint32_t read;
    JTAG_Shift_Data(&write, &read, 8);

    while (1)
    {
        led_blinking_task();
        sleep_ms(1000);
    }
}