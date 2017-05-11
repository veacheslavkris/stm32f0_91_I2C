#include "uart8.h"
#define pTx 2
#define pRx 3

void init_uart8(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	
	//  /* (1) Select AF mode (10) on PC2 and PC3 */
	//  /* (2) AF2 for USART8 signals */
	GpioSetModeUart(GPIOC, 2, 3, ALT_FUNC_2);
	
	/* Enable the peripheral clock USART8 */
  RCC->APB2ENR |= RCC_APB2ENR_USART8EN;
	UartConfigBrrTeReUe(USART8, 6000000, 9600);
}



