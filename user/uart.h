#ifndef UART_H
#define UART_H

#include "stm32f091xc.h"
#include "stdint.h"



//#define UART_BT USART7
//#define UART_PC USART8



#define	CR	0x0D

/****** UART ERRORS ******/

#define UART_ERROR_FREE		0x00000000
#define UART_ERROR_FE			0x00000001
#define UART_ERROR_ORE		0x00000002
#define UART_ERROR_NE			0x00000004
#define UART_ERROR_PE			0x00000008
#define UART_ERROR_TO			0x00000010


#define UART_BT_START_TXEIE		UART_BT->CR1 |= USART_CR1_TXEIE		/* enable RXNE interrupt */
#define UART_BT_STOP_TXEIE		UART_BT->CR1 &= ~USART_CR1_TXEIE	/* disable RXNE interrupt */
#define	UART_BT_CLEAR_TC			UART_BT->ICR |= USART_ICR_TCCF 
#define	UART_BT_CLEAR_TXE			UART_BT->ISR &= ~USART_ISR_TXE

#define UART_PC_START_TXEIE		UART_PC->CR1 |= USART_CR1_TXEIE		/* enable RXNE interrupt */
#define UART_PC_STOP_TXEIE		UART_PC->CR1 &= ~USART_CR1_TXEIE	/* disable RXNE interrupt */
#define UART_PC_CLEAR_TC			UART_PC->ICR |= USART_ICR_TCCF		/* clear TC interrupt */
#define	UART_PC_CLEAR_TXE			UART_PC->ISR &= ~USART_ISR_TXE


#define IS_UART_BT_TC					(UART_BT->ISR & USART_ISR_TC) == USART_ISR_TC
#define IS_UART_BT_RXNE				(UART_BT->ISR & USART_ISR_RXNE) == USART_ISR_RXNE 
#define IS_UART_BT_TXE				(UART_BT->ISR & USART_ISR_TXE) == USART_ISR_TXE

#define IS_UART_PC_TC					(UART_PC->ISR & USART_ISR_TC) == USART_ISR_TC
#define IS_UART_PC_RXNE				(UART_PC->ISR & USART_ISR_RXNE) == USART_ISR_RXNE
#define IS_UART_PC_TXE				(UART_PC->ISR & USART_ISR_TXE) == USART_ISR_TXE

// TODO : replace by const uint32_t with removing from main.c 

#define UART_STATE_STRING_SIZE 24 
#define UART_NAME_SIZE	8
#define UART_MODE_NAME_SIZE			16
#define UART_SUBSTATE_NAME_SIZE	24


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

//typedef enum 
//{
//	STATE_BT_ASKING_PREPARE,
//	STATE_BT_ADDRESS_SENDING,
//	STATE_BT_LISTENING_PREPARE,
//	
//	STATE_DT_ASKING_PREPARE,
//	STATE_DT_ASK_SENDING,
//	STATE_DT_LISTENING_PREPARE,
//	
//	STATE_AC_REPORTING_PREPARE,
//	STATE_AC_REPORT_SENDING,
//	
//	STATE_SENDING_DONE,
//	STATE_ANSW_LISTENING,
//	STATE_ANSW_DONE,
//	STATE_ERR_TIMEOUT,
//	STATE_ERR_FG_UART,
//	STATE_ERR_DATA,
//	STATE_DOWNTIME

//}UartStateEnum;

//typedef struct
//{
//	UartStateEnum enm_us;
//	uint8_t str_us[UART_STATE_STRING_SIZE];
//	
//}UartState;

//typedef enum 
//{
//	MODE_BT_REQUEST,
//	MODE_DT_REQUEST,
//	MODE_AC_REPORT,
//	MODE_IDLE,
//	MODE_ERROR,

//}UartModeEnum;

//typedef struct
//{
//	UartModeEnum enm_mode;
//	uint8_t ary_mode_name[UART_MODE_NAME_SIZE];
//	
//}UartMode;

//typedef enum 
//{
//	FST_PREPARE_SENDING,
//	FST_SENDING,
//	FST_TCNT_SENT,
//	FST_TC,
//	FST_PREPARE_RECEIVING,
//	FST_RECEIVING,
//	FST_TCNT_RECEIVED,
//	FST_FREE,
//	FST_ERR_FG,
//	FST_ERR_DATA,
//	FST_ERR_TIMEOUT

//}UartFuncStateEnum;

//typedef struct
//{
//	UartFuncStateEnum enm_funcstate;
//	uint8_t ary_fst_name[UART_SUBSTATE_NAME_SIZE];
//}UartFuncState;

//typedef struct
//{
//	uint32_t ary_size;
//	uint32_t data_size;
//	uint32_t ix_ary;
//	uint32_t cycles;
//	uint8_t* p_ary_data;
//	
//}UartStructData;	

//typedef struct
//{
//	USART_TypeDef* pUart;
//	UartStructData bufferRX;
//	UartStructData bufferTX;
//	uint8_t ary_uart_name[UART_NAME_SIZE];
//	
//	UartMode* pUartMode;
//	UartFuncState* pUartFuncState;
//}UartHandle;	




//void InitUart_7_8(void);

//void Configure_GPIO_USART7_BT(void);
//void Configure_GPIO_USART8_PC(void);

UartReceivedChar* UartGetReceivedChar(USART_TypeDef* UART);


void UartConfigure(USART_TypeDef* UART, uint32_t apb1_clk, uint32_t brr);
void Configure_USART8_PC(void);
void Configure_USART_7_8_IT(void);


void UartSendCharCR(USART_TypeDef* UART, uint8_t);
void UartSendError(USART_TypeDef* UART, uint8_t);
void UartSendString(USART_TypeDef* UART, char*, uint32_t, CR_STATE);
void UartSendBytes(USART_TypeDef* UART, uint8_t*, uint32_t, CR_STATE);


#endif

