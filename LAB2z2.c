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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =
	GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitTypeDef Przyciski;
	Przyciski.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	Przyciski.GPIO_Mode = GPIO_Mode_IN;
	Przyciski.GPIO_OType = GPIO_OType_PP;
	Przyciski.GPIO_Speed = GPIO_Speed_100MHz;
	Przyciski.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &Przyciski);

	unsigned int j = 0u, i = 0u;
	const unsigned int wait = 5000000u;

	for(;;){
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))
		{
			for(j = 0u; j<wait;j++);
			switch(i%4)
			{
				case 0:
					GPIO_ResetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
					GPIO_SetBits(GPIOD,GPIO_Pin_12 );
					i=1;
					break;
				case 1:
					GPIO_ResetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
					GPIO_SetBits(GPIOD,GPIO_Pin_13 );
					i=2;
					break;
				case 2:
					GPIO_ResetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
					GPIO_SetBits(GPIOD,GPIO_Pin_14 );
					i=3;
					break;
				case 3:
					GPIO_ResetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
					GPIO_SetBits(GPIOD,GPIO_Pin_15 );
					i=0;
					break;
			}
		}
	}
}
