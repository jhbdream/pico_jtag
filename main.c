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

static void cdc_task(void);

int main()
{
    stdio_usb_init();
    jtag_init();

    printf("board init ok!\n");

    while (1)
    {
        printf("board init ok!\n");
        led_blinking_task();
    }
}