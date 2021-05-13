#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/clocks.h"
#include "pio_jtag.h"
#include "bsp/board.h"

#define PIN_TDI 16
#define PIN_TDO 17
#define PIN_TCK 18
#define PIN_TMS 19
#define PIN_RST 20
#define PIN_TRST 21

pio_jtag_inst_t jtag = {
            .pio = pio0,
            .sm = 0
};

void init_pins(uint pin_tck, uint pin_tdi, uint pin_tdo, uint pin_tms, uint pin_rst, uint pin_trst)
{
    bi_decl(bi_4pins_with_names(PIN_TCK, "TCK", PIN_TDI, "TDI", PIN_TDO, "TDO", PIN_TMS, "TMS"));
    bi_decl(bi_2pins_with_names(PIN_RST, "RST", PIN_TRST, "TRST"));

    gpio_clr_mask((1u << pin_tms) | (1u << pin_rst) | (1u << pin_trst));
    gpio_init_mask((1u << pin_tms) | (1u << pin_rst) | (1u << pin_trst));
    gpio_set_dir_masked( (1u << pin_tms) | (1u << pin_rst) | (1u << pin_trst), 0xffffffffu);
}

void jtag_set_clk_freq(const pio_jtag_inst_t *jtag, uint freq_khz) {
    uint clk_sys_freq_khz = clock_get_hz(clk_sys) / 1000;
    uint32_t divider = (clk_sys_freq_khz / freq_khz) / 4;
    divider = (divider < 2) ? 2 : divider;                      //max reliable freq
    pio_sm_set_clkdiv_int_frac(pio0, jtag->sm, divider, 0);
}

void init_jtag_pio(pio_jtag_inst_t* jtag, uint freq, uint pin_tck, uint pin_tdi, uint pin_tdo, uint pin_tms, uint pin_rst, uint pin_trst)
{
    init_pins(pin_tck, pin_tdi, pin_tdo, pin_tms, pin_rst, pin_trst);
    jtag->pin_tdi = pin_tdi;
    jtag->pin_tdo = pin_tdo;
    jtag->pin_tck = pin_tck;
    jtag->pin_tms = pin_tms;
    jtag->pin_rst = pin_rst;
    jtag->pin_trst = pin_trst;
    uint16_t clkdiv = 31;  // around 1 MHz @ 125MHz clk_sys
    pio_jtag_init(jtag->pio, jtag->sm,
                    clkdiv,
                    pin_tck,
                    pin_tdi,
                    pin_tdo
                 );

    jtag_set_clk_freq(jtag, freq);
}

void jtag_init(void)
{
    init_jtag_pio(&jtag, 1000, PIN_TCK, PIN_TDI, PIN_TDO, PIN_TMS, PIN_RST, PIN_TRST);
}