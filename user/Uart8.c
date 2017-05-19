#include "uart8.h"

#define GPIO_UART				GPIOC
#define PIN_TX_D_POS		2
#define PIN_RX_D_POS		3
#define ALT_FUNC				ALT_FUNC_2

#define UART						USART8


void Uart8_Init(void)
{
	/* Enable the peripheral clock of GPIOC */
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	
	GpioSetModeUart(GPIO_UART, PIN_TX_D_POS, PIN_RX_D_POS, ALT_FUNC);

	/* Enable the peripheral clock USART8 */
  RCC->APB2ENR |= RCC_APB2ENR_USART8EN;
	UartConfigBrrTeReUe(UART, 6000000, 9600);
	
//	UartConfig_TC_TXNE_IE_Enable(UART);
//	UartConfig_InitInterrupts(USART3_8_IRQn, 0);

}



