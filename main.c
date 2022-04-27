#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"

#include "pio_jtag.h"

#include <stdio.h>

// Pico
#include "pico/stdlib.h"

// For memcpy
#include <string.h>

// Include descriptor struct definitions
#include "usb_common.h"
// USB register definitions from pico-sdk
#include "hardware/regs/usb.h"
// USB hardware struct definitions from pico-sdk
#include "hardware/structs/usb.h"
// For interrupt enable and numbers
#include "hardware/irq.h"
// For resetting the USB controller
#include "hardware/resets.h"
#include "hardware/gpio.h"

// Device descriptors
#include "dev_lowlevel.h"

#include <DAP.h>
#include <DAP_config.h>

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

extern volatile bool configured;

extern void usb_device_init();
extern void usb_start_transfer(struct usb_endpoint_configuration *ep, uint8_t *buf, uint16_t len);
extern struct usb_endpoint_configuration *usb_get_endpoint_configuration(uint8_t addr);

extern void DAP_Init(void);
extern void DAP_Thread(void);

int main()
{
    uint16_t resp_size;

    board_uart_init();
    stdio_uart_init();
    printf("USB Device Low-Level hardware\n");

    gpio_init(19);
    gpio_set_dir(19, GPIO_OUT);

    DAP_Init();

    usb_device_init();

    // Wait until configured
    while (!configured)
    {
        tight_loop_contents();
    }

    // Get ready to rx from host
    usb_start_transfer(usb_get_endpoint_configuration(EP1_OUT_ADDR), NULL, 64);

    while (1)
    {
        DAP_Thread();
    }
}