/*
 * matrix_keyboard.c
 *
 *  Created on: Jan 7, 2020
 *      Author: Nikolay
 */

//Scan pulse width
#define PULSE_WIDTH 4800 //HCLK ticks (~100us)
#define DEBOUNCE_COUNT 5

#include "matrix_keyboard.h"
#include "stm32f3xx_hal.h"
#include "timer.h"

//Outputs must be open drain
uint32_t row_out[MATRIX_KEYBOARD_ROWS*2]={
(uint32_t)GPIOD, GPIO_PIN_0,
(uint32_t)GPIOD, GPIO_PIN_1,
(uint32_t)GPIOD, GPIO_PIN_2,
(uint32_t)GPIOD, GPIO_PIN_3,
(uint32_t)GPIOD, GPIO_PIN_4,
(uint32_t)GPIOD, GPIO_PIN_5,
(uint32_t)GPIOD, GPIO_PIN_6,
(uint32_t)GPIOD, GPIO_PIN_7,
};

//Inputs must be pulled up
uint32_t column_in[MATRIX_KEYBOARD_COLUMNS*2]={
(uint32_t)GPIOD, GPIO_PIN_8,
(uint32_t)GPIOD, GPIO_PIN_9,
(uint32_t)GPIOD, GPIO_PIN_10,
(uint32_t)GPIOD, GPIO_PIN_11,
(uint32_t)GPIOD, GPIO_PIN_12,
(uint32_t)GPIOD, GPIO_PIN_13,
(uint32_t)GPIOD, GPIO_PIN_14,
(uint32_t)GPIOD, GPIO_PIN_15,
};

//uint8_t key_buf[MATRIX_KEYBOARD_KEY_BUFFER_SIZE]={0};
uint8_t *key_buf;

uint32_t row_i=0, col_i=0, key_i=0; //Row, column and key indexes

uint32_t mask = 0, buf_addr = 0; //vars for key_buf bitwise access

uint16_t debounce_counter=0;

struct tick_timer pulse_timer; //key readout pulse

enum State {idle, pulse};
enum State state = idle;

void  MATRIX_KEYBOARD_Init(uint8_t *buf){
	for(row_i=0 ; row_i<MATRIX_KEYBOARD_ROWS; row_i++)
		HAL_GPIO_WritePin( (GPIO_TypeDef *) row_out[row_i<<1], row_out[(row_i<<1)+1], GPIO_PIN_SET);
	row_i = 0;
	tick_timer_set(&pulse_timer, PULSE_WIDTH);
	key_buf = buf;
}


void MATRIX_KEYBOARD_Scan(void){

	switch(state){
	case idle:{
		HAL_GPIO_WritePin((GPIO_TypeDef *)row_out[row_i<<1], row_out[(row_i<<1)+1], GPIO_PIN_RESET);
		tick_timer_restart(&pulse_timer);
		debounce_counter = 0;
		state = pulse;
		break;
	}
	case pulse:{
		if(HAL_GPIO_ReadPin((GPIO_TypeDef *)column_in[col_i<<1], column_in[(col_i<<1)+1])==GPIO_PIN_RESET){
			debounce_counter++;
		}
		if(tick_timer_expired(&pulse_timer)){
			mask = 1<<(key_i & 0b111);
			buf_addr = key_i>>3;
			if (debounce_counter > DEBOUNCE_COUNT){
				key_buf[buf_addr] |= mask;
			}else{
				key_buf[buf_addr] &= ~mask;
			}
			HAL_GPIO_WritePin((GPIO_TypeDef *)row_out[row_i<<1], row_out[(row_i<<1)+1], GPIO_PIN_SET);
			key_i++;
			if (key_i >= MATRIX_KEYBOARD_SIZE)
				key_i = 0;
			col_i++;
			if (col_i >= MATRIX_KEYBOARD_COLUMNS){
				col_i = 0;
				row_i++;
			}
			if (row_i >= MATRIX_KEYBOARD_ROWS)
				row_i = 0;
			state = idle;
			}
		break;
	}
	}
}
