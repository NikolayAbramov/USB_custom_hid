#ifndef HCLK_FREQUENCY
#define HCLK_FREQUENCY 48000000
#endif

#include "stm32f3xx_hal.h"
#include "inttypes.h"

void delay(uint32_t);
void delay_long(uint32_t);

struct timer {
  uint32_t start;
  uint32_t interval;
  uint8_t run;
};

struct tick_timer {
  uint32_t ms_start;
  uint32_t start;
  uint32_t interval;
  uint8_t run;
};

void timer_set(struct timer *t, uint32_t interval);
void timer_restart(struct timer *t);
uint32_t timer_expired(struct timer *t);

void tick_timer_set(struct tick_timer *t, uint32_t interval);
uint32_t tick_timer_expired(struct tick_timer *t);
void tick_timer_restart(struct tick_timer *t);

