#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/clocks.h"
#include "pio_jtag.h"
#include "bsp/board.h"

pio_jtag_inst_t jtag = DECLEAR_PIO_JTAG(JTAG_PIO, JTAG_SM, PIN_TDI, PIN_TDO, PIN_TCK, PIN_TMS);

pio_jtag_inst_t *pio_jtag = &jtag;

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
    //pio_jtag_init(jtag->pio, pio_jtag->sm, jtag->pin_tck, jtag->pin_tdi, jtag->pin_tdo);
    //pio_set_freq(jtag, freq);
    //pio_sm_set_enabled(jtag->pio, jtag->sm, true);

    pio_jtag_tap_init(pio1, 0, PIN_TCK, PIN_TMS);
    pio_set_freq(pio1, 0, 25000000);
    pio_sm_set_enabled(pio1, 0, true);
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
    pio_sm_put_blocking(pio1, 0, chLength - 1);
    pio_sm_put_blocking(pio1, 0, chCTRStream);
}