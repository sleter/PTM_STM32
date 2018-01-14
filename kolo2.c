/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "tm_stm32f4_lis302dl_lis3dsh.h"

unsigned int counter =0;

int main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_InitTypeDef LEDy;
	LEDy.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	LEDy.GPIO_Mode = GPIO_Mode_IN;
	LEDy.GPIO_OType = GPIO_OType_PP;
	LEDy.GPIO_Speed = GPIO_Speed_100MHz;
	LEDy.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &LEDy);
	LEDy.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	LEDy.GPIO_Mode = GPIO_Mode_OUT;
	LEDy.GPIO_OType = GPIO_OType_PP;
	LEDy.GPIO_Speed = GPIO_Speed_100MHz;
	LEDy.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &LEDy);


	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler = 8399;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_Cmd(TIM3, ENABLE);

	for(;;){
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)){
			while(5){
			if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update)) {
				if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update)) {
					GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
					TIM_ClearFlag(TIM3, TIM_FLAG_Update);
				}
				//GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
				if(counter == 0){
					GPIO_ResetBits(GPIOD, GPIO_Pin_12);
					break;
				}
				counter-=1;
				TIM_ClearFlag(TIM3, TIM_FLAG_Update);
			}
			}
		}
		else{
			if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update)) {
				GPIO_SetBits(GPIOD, GPIO_Pin_12);
				counter+=1;
				TIM_ClearFlag(TIM3, TIM_FLAG_Update);
			}
		}
	}

}
