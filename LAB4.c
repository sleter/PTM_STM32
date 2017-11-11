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


int main(void)
{
	/*
	 * Uruchomienie GPIO
	 * */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_InitTypeDef LEDy;
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
	LEDy.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;
	LEDy.GPIO_Mode = GPIO_Mode_AF;
	LEDy.GPIO_OType = GPIO_OType_PP;
	LEDy.GPIO_Speed = GPIO_Speed_100MHz;
	LEDy.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &LEDy);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler = 83;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_TimeBaseInitTypeDef licznik;
	licznik.TIM_Period = 9999;
	licznik.TIM_Prescaler = 8399;
	licznik.TIM_ClockDivision = TIM_CKD_DIV1;
	licznik.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &licznik);

	TIM_OCInitTypeDef Timer;
	Timer.TIM_OCMode = TIM_OCMode_PWM1;
	Timer.TIM_OutputState = TIM_OutputState_Enable;
	Timer.TIM_Pulse = 0;
	Timer.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM3, &Timer);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_OC3Init(TIM3, &Timer);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_OC4Init(TIM3, &Timer);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	TIM3->CCR1 = 0; //red
	TIM3->CCR3 = 0; //blue
	TIM3->CCR4 = 0; //green

	int i = 0;

	for(;;){
		if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update))
		{
			i+=20;
			if(i>=300)
				i = 0;

			TIM3->CCR1 = i; //red
			//TIM3->CCR3 = i; //blue
			//TIM3->CCR4 = i; //green
			TIM_ClearFlag(TIM2,TIM_FLAG_Update);
		}
	}
}
