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
    JTAG_TAP_TEST_LOGIC_RESET;
    JTAG_TAP_SHIFT_IR;

    while (1)
    {
        led_blinking_task();
        sleep_ms(1000);
    }
}