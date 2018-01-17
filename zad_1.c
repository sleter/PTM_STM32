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

const unsigned int wait = 5000000u;

int main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 2499;
	TIM_TimeBaseStructure.TIM_Prescaler = 8399;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	GPIO_InitTypeDef LEDy;
	LEDy.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10;
	LEDy.GPIO_Mode = GPIO_Mode_OUT;
	LEDy.GPIO_OType = GPIO_OType_PP;
	LEDy.GPIO_Speed = GPIO_Speed_100MHz;
	LEDy.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &LEDy);
	LEDy.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14;
	LEDy.GPIO_Mode = GPIO_Mode_IN;
	LEDy.GPIO_OType = GPIO_OType_PP;
	LEDy.GPIO_Speed = GPIO_Speed_100MHz;
	LEDy.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &LEDy);

	TIM_Cmd(TIM3, ENABLE);

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	GPIO_SetBits(GPIOE,GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10);

	for(;;){

		if(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11))
		{
			for(int j = 0u; j<wait;j++);
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)){
				if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
				{
					GPIO_ToggleBits(GPIOE,GPIO_Pin_7);
					TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				}
			}
			for(int j = 0u; j<wait;j++);
			GPIO_SetBits(GPIOE,GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10);
		}
		if(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12))
		{
			for(int j = 0u; j<wait;j++);
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)){
				if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
				{
					GPIO_ToggleBits(GPIOE,GPIO_Pin_8);
					TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				}
			}
			for(int j = 0u; j<wait;j++);
			GPIO_SetBits(GPIOE,GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10);
		}
		if(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13))
		{
			for(int j = 0u; j<wait;j++);
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)){
				if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
				{
					GPIO_ToggleBits(GPIOE,GPIO_Pin_9);
					TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				}
			}
			for(int j = 0u; j<wait;j++);
			GPIO_SetBits(GPIOE,GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10);
		}
		if(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14))
		{
			for(int j = 0u; j<wait;j++);
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)){
				if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
				{
					GPIO_ToggleBits(GPIOE,GPIO_Pin_10);
					TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				}
			}
			for(int j = 0u; j<wait;j++);
			GPIO_SetBits(GPIOE,GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10);
		}
	}
}




