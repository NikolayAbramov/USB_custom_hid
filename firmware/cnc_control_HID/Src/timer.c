#include "timer.h"

//Software overflow flag
uint32_t ovf=0;

//Short stupid waiting
void delay(uint32_t ticks)
{
	uint32_t start_time;

	start_time = (uint32_t)SysTick->VAL;
	while( 1 )
	{
		if(ovf)
		{
			ovf = 0;
			//if overflow
			if( start_time <= ticks )
				ticks -= start_time;
			else
				break;
			start_time =(uint32_t)SysTick->LOAD;
		}
			
		if( (start_time - (uint32_t)SysTick->VAL) >= ticks )
			break;
	}
}

//Long stupid waiting
void delay_long(uint32_t ms_delay)
{
	uint32_t start_time;
	
	start_time = HAL_GetTick();
	
	while( (HAL_GetTick()-start_time) < ms_delay);
		
}

//ms timer
void timer_set(struct timer *t, uint32_t interval)
{
  t->interval = interval;
  t->start = HAL_GetTick();
	t->run = 1;
}

void timer_restart(struct timer *t)
{
  t->start = HAL_GetTick();
}

uint32_t timer_expired(struct timer *t)
{
  return ((HAL_GetTick() - t->start) >= t->interval) && (t->run);
}

//Core clk tick timer. Interval is in HCLK ticks.
void tick_timer_set(struct tick_timer *t, uint32_t interval)
{
  t->interval = interval;
  tick_timer_restart(t);
}

uint32_t tick_timer_expired(struct tick_timer *t)
{
	uint32_t ms_elapsed, SysTick_val, ms_ticks, ms_ticks1;
	uint8_t expired = 0;

	ms_ticks = HAL_GetTick();
	SysTick_val = SysTick->VAL;
	ms_ticks1 = HAL_GetTick();
	if(ms_ticks!=ms_ticks1){
		ms_ticks = ms_ticks1;
		SysTick_val = SysTick->VAL;
	}

	ms_elapsed = ms_ticks - t->ms_start;

	switch(ms_elapsed){
	case 0:
		if(t->start - SysTick_val >= t->interval ){
			expired = 1;
		}
		break;
	case 1:
		if( SysTick->LOAD - SysTick_val + t->start >= t->interval ){
			expired = 1;
		}
		break;
	default:
		if( (ms_elapsed-1)*SysTick->LOAD + (SysTick->LOAD - SysTick_val) + t->start >= t->interval ){
			expired = 1;
		}
		break;
	}
	if(expired){
		t->run = 0;
		return 1;
	}else{
		return 0;
	}
}

void tick_timer_restart(struct tick_timer *t)
{
	uint32_t ms_ticks;
	t->ms_start = HAL_GetTick();
	t->start	= SysTick->VAL;
	 ms_ticks = HAL_GetTick();
	  if(t->ms_start != ms_ticks){
		  t->ms_start = ms_ticks;
		  t->start	= SysTick->VAL;
	  }
	t->run = 1;
}



