#include "dap_jtag.pio.h"
#include "hardware/address_mapped.h"
#include "hardware/gpio.h"
#include <stdbool.h>
#include <stdint.h>
#include "dap_jtag_pio.h"
#include "hardware/pio.h"

static void pio_dap_jtag_init(PIO pio, uint sm, uint32_t prog_offs, uint32_t freq,
			uint32_t tck, uint32_t tms, uint32_t tdi, uint32_t tdo)
{
	float clkdiv = 48;
	pio_sm_config c = dap_jtag_program_get_default_config(prog_offs);	
	
	sm_config_set_out_pins(&c, tdi, 1);
	sm_config_set_in_pins(&c, tdo);
	sm_config_set_set_pins(&c, tdi, 1);
	sm_config_set_sideset_pins(&c, tck);	
	
	sm_config_set_out_shift(&c, true, true, 32);
	sm_config_set_in_shift(&c, true, false, 32);
	sm_config_set_clkdiv(&c, clkdiv);

	pio_gpio_init(pio, tck);
	pio_gpio_init(pio, tms);
	pio_gpio_init(pio, tdi);
	pio_gpio_init(pio, tdo);

	pio_sm_set_pindirs_with_mask(pio, sm, 1, (1 << tck) | (1 << tms) | (1 << tdi));
	pio_sm_set_pindirs_with_mask(pio, sm, 0, (1 << tdo));
	
	pio_sm_set_consecutive_pindirs(pio, sm, tck, 1, true);
	pio_sm_set_consecutive_pindirs(pio, sm, tms, 1, true);
	pio_sm_set_consecutive_pindirs(pio, sm, tdi, 1, true);
	pio_sm_set_consecutive_pindirs(pio, sm, tdo, 1, false);

	pio_sm_init(pio, sm, prog_offs, &c);
	pio_sm_set_enabled(pio, sm, true);
}

#define PIO_DAP_TCK_PIN 19
#define PIO_DAP_TDI_PIN 21
#define PIO_DAP_TDO_PIN 18
#define PIO_DAP_TMS_PIN 20

struct dap_sequence
{
	uint8_t sequence_info;
	uint8_t tdi_data[8];
}; 

void dap_jtag_init()
{
	uint32_t prog_offs = pio_add_program(pio0, &dap_jtag_program);
	pio_dap_jtag_init(pio0, 0, prog_offs, 1000, PIO_DAP_TCK_PIN, PIO_DAP_TMS_PIN, PIO_DAP_TDI_PIN, PIO_DAP_TDO_PIN);
}

void dap_sequence_test(void)
{
	struct dap_sequence seq = {
		.sequence_info = (7 << 0) | (1 << 6) | (1 << 7),
		.tdi_data[0] = 0xF2,
		.tdi_data[1] = 0x34,
	};

	dap_jtag_init();

#if 1
	{
		pio_sm_put_blocking(pio0, 0, seq.sequence_info);
		pio_sm_put_blocking(pio0, 0, 0xF0);
	}

	while(1);
#endif
}

