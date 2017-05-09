#ifndef UART_H
#define UART_H

#include "stm32f091xc.h"
#include "stdint.h"



#define	CR	0x0D

/****** UART ERRORS ******/

#define UART_ERROR_FREE		0x00000000
#define UART_ERROR_FE			0x00000001
#define UART_ERROR_ORE		0x00000002
#define UART_ERROR_NE			0x00000004
#define UART_ERROR_PE			0x00000008
#define UART_ERROR_TO			0x00000010


typedef enum 
{
	CR_OFF,
	CR_ON
}CR_STATE;

typedef struct
{
	uint32_t uart_error_state;
	uint8_t chartoreceive;
	
} UartReceivedChar;



UartReceivedChar* UartGetReceivedChar(USART_TypeDef* UART);


void UartConfigBrrTeReUe(USART_TypeDef* UART, uint32_t apb1_clk, uint32_t brr);


void UartSendCharCR(USART_TypeDef* UART, uint8_t);
void UartSendError(USART_TypeDef* UART, uint8_t);
void UartSendString(USART_TypeDef* UART, char*, uint32_t, CR_STATE);
void UartSendBytes(USART_TypeDef* UART, uint8_t*, uint32_t, CR_STATE);


#endif

