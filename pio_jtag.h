
#ifndef _PIO_JTAG_H
#define _PIO_JTAG_H

#include "hardware/pio.h"
#include "jtag.pio.h"

typedef struct pio_jtag_inst {
    PIO pio;
    uint sm;
    uint pin_tdi;
    uint pin_tdo;
    uint pin_tck;
    uint pin_tms;
    uint pin_rst;
    uint pin_trst;
} pio_jtag_inst_t;

void jtag_init(void);

#endif