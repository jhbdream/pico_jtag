
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
} pio_jtag_inst_t;

extern pio_jtag_inst_t *pio_jtag;

#define DECLEAR_PIO_JTAG(_pio, _sm, _pin_tdi, _pin_tdo, _pin_tck, _pin_tms) \
    {   \
        .pio = _pio,    \
        .sm = _sm,      \
        .pin_tdi = _pin_tdi,    \
        .pin_tdo = _pin_tdo,    \
        .pin_tck = _pin_tck,    \
        .pin_tms = _pin_tms,    \
    }

#define JTAG_PIO (pio0)
#define JTAG_SM (0)

#define PIN_TRST 22
#define PIN_TDI 21
#define PIN_TMS 20
#define PIN_TCK 19
/* 需要飞线 */
#define PIN_TDO 26

#define JTAG_FREQ (1000000)

void jtag_init(pio_jtag_inst_t *jtag, uint freq);
void JTAG_TAP_Control(uint8_t chCTRStream,uint8_t chLength);
void JTAG_Shift_Data(uint32_t *pchOutBuffer, uint32_t *pchInBuffer, uint16_t wLength);
uint32_t JTAG_Read_Device_Identification(void);

#define JTAG_TAP_TEST_LOGIC_RESET              JTAG_TAP_Control(0x1F,6);
#define JTAG_TAP_SHIFT_IR                      JTAG_TAP_Control(0x03,4);
#define JTAG_TAP_RETURN_RUN_TEST_IDEL          JTAG_TAP_Control(0x01,2);
#define JTAG_TAP_ENTER_SHIFT_DR_FROM_SHIFT_IR  JTAG_TAP_Control(0x03,4);
#define JTAG_TAP_SHIFT_DR                      JTAG_TAP_Control(0x01,3);

#endif