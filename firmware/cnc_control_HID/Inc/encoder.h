/*
 * encoder.h
 *
 *  Created on: 10 февр. 2020 г.
 *      Author: Nikolay
 */

#ifndef ENCODER_H_
#define ENCODER_H_

//Maximum 8 channels
#define ENCODER_CHANNELS 2

//Counters
//extern uint32_t ENCODER_counters[ENCODER_CHANNELS];

//Accepts pointer to 16bit counters
//Buffer size must be equal to ENCODER_CHANNELS
void ENCODER_Init(uint16_t *);

//Capture state of encoders
void ENCODER_Capture(void);

#endif /* ENCODER_H_ */
