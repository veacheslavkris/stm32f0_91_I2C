 /* Includes ------------------------------------------------------------------*/
//#include "stm32f0xx.h"
#include "uart.h"

UartReceivedChar structReceivedChar;

void send_char(USART_TypeDef* UART, uint8_t ch);
void send_char_cr(USART_TypeDef* UART, uint8_t);
void send_cr(USART_TypeDef* UART);

//void InitUart_7_8(void)
//{
//	/* Enable the peripheral clock of GPIOC */
//  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
//	
//	Configure_GPIO_USART7_BT();
//	Configure_USART7_BT();
//	Configure_GPIO_USART8_PC();
//	Configure_USART8_PC();
//	Configure_USART_7_8_IT();

//}

//// BT connection
//void Configure_GPIO_USART7_BT(void)
//{
//  /* Enable the peripheral clock of GPIOC */
////  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
//	
//  /* GPIO configuration for USART7 signals */
//  /* (1) Select AF mode (10) on PC0 and PC1 */
//  /* (2) AF1 for USART7 signals */
//  GPIOC->MODER = (GPIOC->MODER & ~(GPIO_MODER_MODER0|GPIO_MODER_MODER1))\
//                 | (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1); /* (1) */
//  GPIOC->AFR[0] = (GPIOC->AFR[0] &~ (GPIO_AFRL_AFRL0 | GPIO_AFRL_AFRL1))\
////                  | (1) | (1 << (1 * 4)); /* (2) */
//									| 0x0011; /* 0001 0001 (2) */
//									
//}


//void Configure_GPIO_USART8_PC(void)
//{
//  /* Enable the peripheral clock of GPIOC */
////  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
//	
//  /* GPIO configuration for USART8 signals */
//  /* (1) Select AF mode (10) on PC2 and PC3 */
//  /* (2) AF2 for USART8 signals */
//  GPIOC->MODER = (GPIOC->MODER & ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3))\
//                 | (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1); /* (1) */
//  GPIOC->AFR[0] = (GPIOC->AFR[0] &~ (GPIO_AFRL_AFRL2 | GPIO_AFRL_AFRL3))\
////                  | (2 << (2 * 4)) | (2 << (3 * 4)); /* (2) */
//									| 0x2200; /* 0010 0010 0000 0000 (2) */
//									
//}


//void Configure_USART_7_8_IT()
//{
//  /* Configure IT */
//  /* (3) Set priority for USART7_IRQn */
//  /* (4) Enable USART7_IRQn */
//  NVIC_SetPriority(USART3_8_IRQn, 0); /* (3) */
//  NVIC_EnableIRQ(USART3_8_IRQn); /* (4) */
//}


void UartConfigure(USART_TypeDef* UART, uint32_t apb1_clk, uint32_t brr)
{
  /* Enable the peripheral clock USART7 */
//  RCC->APB2ENR |= RCC_APB2ENR_USART7EN;

  /* Configure UART */
  /* (1) oversampling by 16, 38400 baud */
  /* (2) 8 data bit, 1 start bit, 1 stop bit, no parity, transmit/receive */
// 	UART->BRR = 6000000 / 38400; /* (1) */

 	UART->BRR = apb1_clk / brr; /* (1) */

	
	UART->CR1 = USART_CR1_TE |USART_CR1_RE | USART_CR1_UE; /* (2) */
	
  /* polling idle frame Transmission */
	while((UART->ISR & USART_ISR_TC) != USART_ISR_TC) continue;
//  { 
//    /* add time out here for a robust application */
//  }
  	
	UART->ICR |= USART_ICR_TCCF;		/* clear TC flag */
 
//	UART->CR1 |= USART_CR1_TCIE;		/* enable TC interrupt */
//	UART->CR1 |= USART_CR1_RXNEIE;	/* enable RXNE interrupt */
		
	//Configure_USART_7_8_IT
}

__INLINE void send_char(USART_TypeDef* UART, uint8_t ch)
{
	UART->TDR = ch;
	while((UART->ISR & USART_ISR_TXE)!=USART_ISR_TXE) continue;
}

//

__INLINE void send_char_cr(USART_TypeDef* UART, uint8_t ch)
{
	send_char(UART, ch);
	
	send_cr(UART);
}

//

__INLINE void send_cr(USART_TypeDef* UART)
{
	UART->TDR = CR;
	while((UART->ISR & USART_ISR_TXE)!=USART_ISR_TXE) continue;
}
//

void UartSendCharCR(USART_TypeDef* UART, uint8_t ch)
{
	send_char_cr(UART, ch);
}
//

UartReceivedChar* UartGetReceivedChar(USART_TypeDef* UART)
{
	structReceivedChar.uart_error_state = 0;
	structReceivedChar.chartoreceive = 0;
	
	if((UART->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
	{
		/****** B. RECEIVING STATE CHECK ******/
			
		/****** ERROR CHECK ******/

		if((UART->ISR & USART_ISR_FE) == USART_ISR_FE)
		{
			UART->ICR|=USART_ICR_FECF;
			structReceivedChar.uart_error_state|=UART_ERROR_FE;
		}

		if((UART->ISR & USART_ISR_ORE) == USART_ISR_ORE)
		{
			UART->ICR|=USART_ICR_ORECF;
			structReceivedChar.uart_error_state|=UART_ERROR_ORE;
		}

		if((UART->ISR & USART_ISR_NE) == USART_ISR_NE)
		{
			UART->ICR|=USART_ICR_NCF;
			structReceivedChar.uart_error_state|=UART_ERROR_NE;
		}
					
		structReceivedChar.chartoreceive = (uint8_t)(UART->RDR); // Receive data, clear flag
		
		if((UART->ISR & USART_ISR_ORE) == USART_ISR_ORE)
		{
			UART->ICR|=USART_ICR_ORECF;
			structReceivedChar.uart_error_state|=UART_ERROR_ORE;
		}
	
	}
	
	return &structReceivedChar;
}
//

void UartSendError(USART_TypeDef* UART, uint8_t err_char)
{
	char* err_string = "error: ";
	uint32_t ix = 0;
	uint32_t count = 7; //sizeof(err_string);
	
	for(ix = 0; ix < count; ix++)
	{
			send_char(UART, err_string[ix]);
	}
	
	send_char_cr(UART, err_char);
	
	
	
}
//

void UartSendString(USART_TypeDef* UART, char* string, uint32_t count, CR_STATE cr_state)
{
	uint32_t ix = 0;
	
	for(ix = 0; ix < count; ix++)
	{
			send_char(UART, string[ix]);
	}
	
	if(cr_state == CR_ON) send_cr(UART);
}
//

void UartSendBytes(USART_TypeDef* UART, uint8_t* bytes, uint32_t count, CR_STATE cr_state)
{
	uint32_t ix = 0;
	
	for(ix = 0; ix < count; ix++)
	{
			send_char(UART, bytes[ix]);
	}
	
	if(cr_state == CR_ON) send_cr(UART);
}

//


