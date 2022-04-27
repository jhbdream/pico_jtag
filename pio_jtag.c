#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "pio_jtag.h"

pio_jtag_inst_t jtag = DECLEAR_PIO_JTAG(JTAG_PIO, JTAG_SM, PIN_TDI, PIN_TDO, PIN_TCK, PIN_TMS);

pio_jtag_inst_t *pio_jtag = &jtag;


static inline void pio_jtag_tap_init(PIO pio, uint sm, uint pin_tck, uint pin_tms)
{
    uint prog_offs = pio_add_program(pio, &jtag_tap_program);
    pio_sm_config c = jtag_tap_program_get_default_config(prog_offs);

    sm_config_set_out_pins(&c, pin_tms, 1);
    sm_config_set_sideset_pins(&c, pin_tck);

    sm_config_set_out_shift(&c, true, false, 32);

    pio_sm_set_pins_with_mask(pio, sm,  (1u << pin_tck) | (1u << pin_tms),
                            (1u << pin_tck) | (1u << pin_tms));

    pio_sm_set_pindirs_with_mask(pio, sm,  (1u << pin_tck) | (1u << pin_tms),
                            (1u << pin_tck) | (1u << pin_tms));

    pio_gpio_init(pio, pin_tck);
    pio_gpio_init(pio, pin_tms);

    pio_sm_init(pio, sm, prog_offs, &c);
}

static inline void pio_jtag_date_init(PIO pio, uint sm, uint pin_tck, uint pin_tdi, uint pin_tdo)
{
    uint prog_offs = pio_add_program(pio, &jtag_date_program);
    pio_sm_config c = jtag_date_program_get_default_config(prog_offs);

    sm_config_set_out_pins(&c, pin_tdi, 1);
    sm_config_set_in_pins(&c, pin_tdo);
    sm_config_set_sideset_pins(&c, pin_tck);

    sm_config_set_out_shift(&c, true, false, 32);
    sm_config_set_in_shift(&c, true, false, 32);

    pio_sm_set_pins_with_mask(pio, sm, (1u << pin_tck) | (1u << pin_tdi),
                         (1u << pin_tck) | (1u << pin_tdi));

    pio_sm_set_pindirs_with_mask(pio, sm, (1u << pin_tck) | (1u << pin_tdi), (1u << pin_tck) | (1u << pin_tdi));
    pio_sm_set_pindirs_with_mask(pio, sm, 0, (1u << pin_tdo));

    pio_gpio_init(pio, pin_tck);
    pio_gpio_init(pio, pin_tdi);
    pio_gpio_init(pio, pin_tdo);

    pio_sm_init(pio, sm, prog_offs, &c);
}

/**
 * @brief 设置执行一条指令的周期，或称为指令频率
 *
 * @param jtag
 * @param freq
 */
void pio_set_freq(PIO pio, uint sm, uint freq)
{
    uint sys_freq,divider;

    sys_freq = clock_get_hz(clk_sys);

    divider = (sys_freq / freq);

    pio_sm_set_clkdiv(pio, sm, divider);
}

void jtag_init(pio_jtag_inst_t *jtag, uint freq)
{

    pio_jtag_tap_init(pio0, 0, PIN_TCK, PIN_TMS);
    pio_set_freq(pio0, 0, 40000000);
    pio_sm_set_enabled(pio0, 0, true);

    pio_jtag_date_init(pio0, 1, PIN_TCK, PIN_TDI, PIN_TDO);
    pio_set_freq(pio0, 1, 40000000);
    pio_sm_set_enabled(pio0, 1, true);
}

/***********************************************************
*   函数说明：JTAG TAP 状态机控制函数                      *
*   输入：    控制序列，序列长度                           *
*   输出：    无                                           *
*   调用函数：无                                           *
* -------------------------------------------------------- *
*   使用说明                                               *
*           1、 TMS在TCK上升边沿输出状态控制量。           *
*           2、 连续5个TCK周期在TMS上输出高电平将进入Test- *
*               Logic-Reset模式。                          *
*           3、 使用该函数时，请将状态机跳转以后TMS需要保  *
*               持的电平也作为一个有效输入加入到序列的末尾 *
*               但描述序列长度的数值不需要相应的增加。     *
***********************************************************/
void JTAG_TAP_Control(uint8_t chCTRStream,uint8_t chLength)
{
    pio_sm_put_blocking(pio0, 0, chLength - 1);
    pio_sm_put_blocking(pio0, 0, chCTRStream);
    pio_sm_get_blocking(pio0, 0);
}

void LONG_JTAG_TAP_Control(uint32_t tap, uint8_t length)
{
    pio_sm_put_blocking(pio0, 0, length - 1);
    pio_sm_put_blocking(pio0, 0, tap);
    pio_sm_get_blocking(pio0, 0);
}

void JTAG_Shift_Data(uint32_t *pchOutBuffer, uint32_t *pchInBuffer, uint16_t wLength)
{

    static uint32_t tmp;

    if(pchOutBuffer != NULL)
    {
        tmp = *pchOutBuffer;
    }

    pio_sm_put_blocking(pio0, 1, wLength - 1);
    pio_sm_put_blocking(pio0, 1, tmp);

    tmp = pio_sm_get_blocking(pio0, 1);
    if(pchInBuffer != NULL)
    {
        *pchInBuffer = tmp;
    }
}

#define OPCODE_IDCODE                  0x0e

/***********************************************************
*   函数说明：设备ID信息读取函数                           *
*   输入：    无                                           *
*   输出：    读取到的ID                                   *
*   调用函数：无                                           *
***********************************************************/
uint32_t JTAG_Read_Device_Identification(void)
{
    uint32_t dwTempData = 0x12345678;
    JTAG_TAP_TEST_LOGIC_RESET;

    JTAG_TAP_SHIFT_IR
    {
        uint32_t chTempData = 0x01;
        JTAG_Shift_Data(&chTempData, NULL, 4);
    }

    JTAG_TAP_TEST_LOGIC_RESET

    JTAG_TAP_SHIFT_DR
    {
        JTAG_Shift_Data((uint32_t *)&dwTempData,(uint32_t *)&dwTempData,32);
    }

    JTAG_TAP_TEST_LOGIC_RESET

    return dwTempData;
}