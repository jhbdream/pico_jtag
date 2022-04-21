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

#include <usb_descriptors.h>
#include <DAP.h>

void board_jtag_init(void)
{
    jtag_init(pio_jtag, 1000000);
    uint32_t idcode = JTAG_Read_Device_Identification();
    printf("idcode: 0x%032x", idcode);
    DAP_Setup();
}

void board_uart_init(void)
{
    // Initialise UART 0
    uart_init(uart0, 115200);

    // Set the GPIO pin mux to the UART - 0 is TX, 1 is RX
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);

    uart_puts(uart0, "Hello world!");
    stdio_uart_init();
}

int main()
{
    board_init();
    tusb_init();
    stdio_uart_init();

    while (1)
    {
        led_blinking_task();
        tud_task(); // tinyusb device task
    }
}