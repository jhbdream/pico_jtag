#ifndef _LED_H_
#define _LED_H_

enum  {
  BLINK_INIT = 1000,
  BLINK_CONNET = 500,
  BLINK_RUNNING = 200,
};

extern uint32_t blink_interval_ms;
void led_blinking_task(void);
void set_blink_state(uint32_t mode);

#endif