 /* Includes ------------------------------------------------------------------*/
#include "a_uart.h"

UartReceivedChar structReceivedChar;

void send_char(USART_TypeDef* UART, uint8_t ch);
void send_char_cr(USART_TypeDef* UART, uint8_t);
void send_cr(USART_TypeDef* UART);

// 

void UartConfigBrrTeReUe(USART_TypeDef* UART, uint32_t apb1_clk, uint32_t brr)
{
  /* Configure UART */
  /* (1) oversampling by 16, 9600 baud */
  /* (2) 8 data bit, 1 start bit, 1 stop bit, no parity, transmit/receive */

 	UART->BRR = apb1_clk / brr; /* (1) */
	
	UART->CR1 = USART_CR1_TE |USART_CR1_RE | USART_CR1_UE; /* (2) */
	
  /* polling idle frame Transmission */
	while((UART->ISR & USART_ISR_TC) != USART_ISR_TC) continue;

  	
	UART->ICR |= USART_ICR_TCCF;		/* clear TC flag */
}

void UartConfig_TC_TXNE_Enable(USART_TypeDef* UART)
{
	UART->CR1 |= USART_CR1_TCIE;		/* enable TC interrupt */
	UART->CR1 |= USART_CR1_RXNEIE;	/* enable RXNE interrupt */
	

}

void UartConfig_InitInterrupts(IRQn_Type irq, uint32_t priority)
{
	/* Configure IT */
  /* (3) Set priority for USARTx_IRQn */
  /* (4) Enable USARTx_IRQn */
  NVIC_SetPriority(irq, priority); /* (3) */
  NVIC_EnableIRQ(irq); /* (4) */
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


