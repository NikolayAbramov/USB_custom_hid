/*
 * matrix_keyboard.h
 *
 *  Created on: Jan 7, 2020
 *      Author: Nikolay
 */

#ifndef MATRIX_KEYBOARD_H_
#define MATRIX_KEYBOARD_H_
#endif /* MATRIX_KEYBOARD_H_ */

#define MATRIX_KEYBOARD_ROWS 8
#define MATRIX_KEYBOARD_COLUMNS 8
#define MATRIX_KEYBOARD_SIZE 64 //MATRIX_KEYBOARD_ROWS*MATRIX_KEYBOARD_COLUMNS
#define MATRIX_KEYBOARD_KEY_BUFFER_SIZE 8 //bytes 1 bit per key

#include "inttypes.h"

//extern uint8_t key_buf[MATRIX_KEYBOARD_KEY_BUFFER_SIZE];

void MATRIX_KEYBOARD_Init(uint8_t *buf);
void MATRIX_KEYBOARD_Scan(void);
