/*
 * encoder.c
 *
 *  Created on: Jan 26, 2020
 *      Author: Nikolay
 */

#include "stm32f3xx_hal.h"
#include "encoder.h"

//A quadrature GPIO connections
uint32_t A_in[ENCODER_CHANNELS*2]={
(uint32_t)GPIOA, GPIO_PIN_15,
(uint32_t)GPIOC, GPIO_PIN_0
};

//B quadrature GPIO connections
uint32_t B_in[ENCODER_CHANNELS*2]={
(uint32_t)GPIOB, GPIO_PIN_3,
(uint32_t)GPIOC, GPIO_PIN_1
};


//Counters
//uint32_t ENCODER_counters[ENCODER_CHANNELS] = {0};
uint16_t *ENCODER_counters;
uint8_t fwd_cnt[ENCODER_CHANNELS] = {0};
uint8_t back_cnt[ENCODER_CHANNELS] = {0};

//Encoders states
uint8_t AB_old[2] = {0}, AB_new[2] = {0};

static void Read(void);

//Read encoder state
//Returns AB array where AB[0] is A and AB[1] is B quadratures
static void Read(void){
	uint8_t i;
	uint8_t state;

	AB_new[0] = 0;
	AB_new[1] = 0;

	for(i=0 ; i<ENCODER_CHANNELS; i++){
		state = HAL_GPIO_ReadPin((GPIO_TypeDef *)A_in[i<<1], A_in[(i<<1)+1]);
		AB_new[0] |= state << i;
		state = HAL_GPIO_ReadPin((GPIO_TypeDef *)B_in[i<<1], B_in[(i<<1)+1]);
		AB_new[1] |= state << i;
	}
}

void ENCODER_Init(uint16_t * buf){
	ENCODER_counters = buf;
}

	void ENCODER_Capture(void)
		{
			uint8_t fwd, back, i;

			Read();

			//Forward direction
			fwd = (AB_old[1] ^ AB_new[0]) & ~(AB_old[0] ^ AB_new[1]);
			//Backward direction
			back = (AB_old[0] ^ AB_new[1]) & ~(AB_old[1] ^ AB_new[0]);

			AB_old[0] = AB_new[0];
			AB_old[1] = AB_new[1];

			for(i = 0; i<ENCODER_CHANNELS; i++)
			{
				if(fwd & 1)
					//ENCODER_counters[i]++;
					fwd_cnt[i]++;
				else if(back & 1)
					//ENCODER_counters[i]--;
					back_cnt[i]++;
				fwd >>= 1;
				back >>= 1;

				if( !(AB_new[0] & (1<<i)) && !(AB_new[1] & (1<<i)) ){
					if(fwd_cnt[i]==4)
						ENCODER_counters[i]++;
					else if(back_cnt[i]==4)
						ENCODER_counters[i]--;
					fwd_cnt[i]=0;
					back_cnt[i]=0;

				}
			}
		}

