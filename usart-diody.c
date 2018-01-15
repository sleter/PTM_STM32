#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void USART_Config(void)
{
	// wlaczenie taktowania wybranego portu
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	// wlaczenie taktowania wybranego ukladu USART
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// konfiguracja linii Rx i Tx
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// ustawienie funkcji alternatywnej dla pinów (USART)
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
	USART_InitTypeDef USART_InitStructure;

	// predkosc transmisji (mozliwe standardowe opcje: 9600, 19200, 38400, 57600, 115200, ...)
	USART_InitStructure.USART_BaudRate = 9600;
	// dlugosc slowa (USART_WordLength_8b lub USART_WordLength_9b)
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// liczba bitów stopu (USART_StopBits_1, USART_StopBits_0_5, USART_StopBits_2, USART_StopBits_1_5)
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// sprawdzanie parzystosci (USART_Parity_No, USART_Parity_Even, USART_Parity_Odd)
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// sprzetowa kontrola przeplywu (USART_HardwareFlowControl_None, USART_HardwareFlowControl_RTS, USART_HardwareFlowControl_CTS, USART_HardwareFlowControl_RTS_CTS)
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// tryb nadawania/odbierania (USART_Mode_Rx, USART_Mode_Rx )
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// konfiguracja
	USART_Init(USART3, &USART_InitStructure);

	// wlaczenie ukladu USART
	USART_Cmd(USART3, ENABLE);

}

void USART_IRQ(USART_TypeDef* USARTx, uint8_t USARTx_IRQn, uint8_t priority )
{
	//struktura do konfiguracji kontrolera NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	// wlaczenie przerwania zwiazanego z odebraniem danych (pozostale zrodla przerwan zdefiniowane sa w pliku stm32f4xx_usart.h)
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// konfiguracja kontrolera przerwan
	NVIC_Init(&NVIC_InitStructure);
	// wlaczenie przerwan od ukladu USART
	NVIC_EnableIRQ(USARTx_IRQn);
}

void Timer_Config(uint16_t presc, uint32_t period, uint32_t RCC_APB1Periph_TIMx, TIM_TypeDef* TIMx, FunctionalState state )
{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE );
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //84mhz 84000000
		TIM_TimeBaseStructure.TIM_Prescaler = presc;
		TIM_TimeBaseStructure.TIM_Period = period;
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
		TIM_Cmd(TIMx, state);
}

void Timer_IRQ(TIM_TypeDef* TIMx, uint8_t TIMx_IRQn, uint8_t priority )
{
	// ustawienie trybu pracy priorytetów przerwan
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitTypeDef NVIC_InitStructure;
	// numer przerwania
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;
	// priorytet glówny
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = priority;
	// subpriorytet
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	// uruchom dany kanal
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// zapisz wypelniona strukture do rejestrów
	NVIC_Init(&NVIC_InitStructure);

	// wyczyszczenie przerwania od timera 3 (wystapilo przy konfiguracji timera)
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
	// zezwolenie na przerwania od przepelnienia dla timera 3
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);

}

void GPIO_Config_wys( ){
	/* GPIOD Periph clock enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

		GPIO_InitTypeDef  GPIO_InitStructure;

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);


		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

			GPIO_InitTypeDef  GPIO_InitStructure1;
			GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_15;
			GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure1.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(GPIOB, &GPIO_InitStructure1);
			GPIO_SetBits(GPIOB, GPIO_Pin_15);
}

void zero(){
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
	GPIO_SetBits(GPIOE, GPIO_Pin_6);
}

void jeden(){
	GPIO_ResetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_2);
	GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
}

void dwa(){
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6);
	GPIO_SetBits(GPIOE, GPIO_Pin_2 | GPIO_Pin_5);

}

void trzy(){
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_SetBits(GPIOE, GPIO_Pin_4 | GPIO_Pin_5);

}

void cztery(){
	GPIO_ResetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6);
	GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4);

}

void piec(){
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6);
	GPIO_SetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_4);

}

void szesc(){
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
	GPIO_SetBits(GPIOE, GPIO_Pin_1);

}

void siedem(){
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
	GPIO_SetBits(GPIOE, GPIO_Pin_6 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);

}

void osiem(){
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
}

void dziewiec(){
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6);
	GPIO_SetBits(GPIOE, GPIO_Pin_4);

}

void liczby(char a){
	switch (a) {
	case '0':
		zero();
		break;
	case '1':
		jeden();
		break;
	case '2':
		dwa();
		break;
	case '3':
		trzy();
		break;
	case '4':
		cztery();
		break;
	case '5':
		piec();
		break;
	case '6':
		szesc();
		break;
	case '7':
		siedem();
		break;
	case '8':
		osiem();
		break;
	case '9':
		dziewiec();
		break;
	default:
		GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
	}
}

uint8_t usartGetChar(void){
	while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)==RESET);
	return USART_ReceiveData(USART3);
}
void TIM4_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
         	{
            	USART_SendData(USART3, 'A');

            	GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
                TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
         	}
}

void GPIO_Config(uint32_t Gpio, uint32_t Pins){
	/* GPIOD Periph clock enable */
		RCC_AHB1PeriphClockCmd(Gpio, ENABLE);

		GPIO_InitTypeDef  GPIO_InitStructure;
		/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin = Pins;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void USART3_IRQHandler(void)
{
	// sprawdzenie flagi zwiazanej z odebraniem danych przez USART
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
       {
		//GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		// odebrany bajt znajduje sie w rejestrze USART3->DR
		char a = usartGetChar();
		liczby(a);
	}
}

int main(void)
{
	//GPIO_Config(RCC_AHB1Periph_GPIOD,GPIO_Pin_12 );
	GPIO_Config_wys();
	USART_Config();
	USART_IRQ(USART3,USART3_IRQn,0);
	Timer_Config(8399, 9999, RCC_APB1Periph_TIM4, TIM4, ENABLE);
	//Timer_IRQ(TIM4,TIM4_IRQn, 0);



	for(;;) {

	}
}
