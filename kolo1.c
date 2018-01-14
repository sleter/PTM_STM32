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
			
int ADC_Result = 0;
double Voltage = 0.0;

int main(void)
{

	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	//GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);

	GPIO_InitTypeDef LEDy;/*
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	LEDy.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_6 | GPIO_Pin_9 ;
	LEDy.GPIO_Mode = GPIO_Mode_AF;
	LEDy.GPIO_OType = GPIO_OType_PP;
	LEDy.GPIO_Speed = GPIO_Speed_100MHz;
	LEDy.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &LEDy);*/

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
	LEDy.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	LEDy.GPIO_Mode = GPIO_Mode_AF;
	LEDy.GPIO_OType = GPIO_OType_PP;
	LEDy.GPIO_Speed = GPIO_Speed_100MHz;
	LEDy.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &LEDy);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 99;
	TIM_TimeBaseStructure.TIM_Prescaler = 8399;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_OCInitTypeDef Timer;
	Timer.TIM_OCMode = TIM_OCMode_PWM1;
	Timer.TIM_OutputState = TIM_OutputState_Enable;
	Timer.TIM_Pulse = 0;
	Timer.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM4, &Timer);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM4, &Timer);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM4, &Timer);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM4, &Timer);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_84Cycles);
	ADC_Cmd(ADC1, ENABLE);

	TIM_Cmd(TIM4, ENABLE);

	TIM4->CCR2 = 0;
	TIM4->CCR1 = 0;
	TIM4->CCR3 = 0;
	TIM4->CCR4 = 0;

	for(;;){

		ADC_SoftwareStartConv(ADC1);
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		ADC_Result = ADC_GetConversionValue(ADC1);

		Voltage = ADC_Result/4095.0 * 9900.0;

		if((int)Voltage<=2100){
			TIM4->CCR1 = 50;
			TIM4->CCR2 = 50;
			TIM4->CCR3 = 0;
		}
		else if((int)Voltage<=4200 && (int)Voltage>2100){
			TIM4->CCR1 = 100;
			TIM4->CCR2 = 50;
			TIM4->CCR3 = 50;
		}
		else if((int)Voltage<=6300 && (int)Voltage>4200){
			TIM4->CCR1 = 100;
			TIM4->CCR2 = 100;
			TIM4->CCR3 = 50;
		}
		else if((int)Voltage>6300){
			TIM4->CCR1 = 100;
			TIM4->CCR2 = 100;
			TIM4->CCR3 = 100;
		}

	}

}
