/**
  ******************************************************************************
  * @file    st32f0_91_I2C/main.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   This code example shows how to configure a GPIO to use it
  *          as external interrupt
  */
 

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
	uint32_t state_run = 0;
	float cur_temp = 0;
	StructFloatToBcd structFloatToBcd;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f072xb.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */


	SetSysClock();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);/* 1ms config with HSE 8MHz/system 48Mhz*/

	RtcLse32768_Init();
	
//	TMP275_Init();
	EEPROM_2Kb_Init();
	
	/* MAX 7219 */ 
	Max7219_Init();

	Max7219_ClearAllDigits();
	
	DelaySystick(1000);
	
	Max7219_ShowAtPositionNumber(11, 0);

	DelaySystick(2000);
	Max7219_ShowAtPositionNumber(0, 15);
	DelaySystick(2000);

	Max7219_ClearAllDigits();

	Max7219_ShowAtPositionNumber(0,0);
	Max7219_ShowAtPositionNumber(4,0);

	BtnPc13_Init();
	
	LedPA5_Init();
	
	LED_ON;
	DelaySystick(2000);
	LED_OFF;

	structFloatToBcd.float_count=2;
	
	while (1) /* Infinite loop */
  {
		if(state_run == 1)
		{
//			TMP275ExecutionResult* tmp275result = TMP275GetTemperature();
//						
//			Max7219_ClearAllDigits();
//			DelaySystick(1500);
//			
//			if(tmp275result->execState == MEASUREMENT_SUCCESSFULL)
//			{
//				structFloatToBcd.flt = tmp275result->temprt;
//				ConvertFloatToBcd(&structFloatToBcd);
//			
//				DisplayMax7219_TT_tt(0, &structFloatToBcd);
//			}
//			else show_err_on_display_0();
			
			volatile I2CStructHandle* eepromHandle;

			I2CStateEnum status;
			
			volatile uint8_t* p_rx_buff;
			volatile uint8_t* p_tx_buff;
			
			uint32_t count = 20;
			
			eepromHandle = EEPROM_GetHandle();
			
			EEPROM_ClearRxTxBuffers();

			p_rx_buff = EEPROM_GetRxBufferArray();
//			p_tx_buff = EEPROM_GetTxBufferArray();
//			
//			eepromHandle->TxBuff.p_ary_data[10]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[11]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[12]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[13]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[14]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[15]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[16]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[17]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[18]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[19]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[20]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[21]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[22]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[23]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[24]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[25]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[26]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[27]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[28]=0xAA;
//			eepromHandle->TxBuff.p_ary_data[29]=0xAA;
//			
//			status =EEPROM_Write(10, 20);			
			
			
			status =  EEPROM_Read(10, 20);
			
//			p_rx_buff = EEPROM_GetRxBufferArray();
			

			
			
			state_run = 0;
		}

  }

}

void cycle_digits(void)
{
	static uint32_t cnt = 0;
	
	static uint32_t ix_digit_4 = 0;
	
	if(cnt == 10)
	{
		cnt = 0;
		
		ix_digit_4++;
		
		if(ix_digit_4 == 10)ix_digit_4 = 0;
	}

	
	Max7219_ShowAtPositionNumber(0, cnt);
	Max7219_ShowAtPositionNumber(4, ix_digit_4);

	cnt++;

}







/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

void NMI_Handler(void)
{
}
//
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
//
void SVC_Handler(void)
{
}
//
void PendSV_Handler(void)
{
}
//

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f072xb.s).                                               */
/******************************************************************************/

void EXTI4_15_IRQHandler(void)
{
  if ((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13)  /* Check line 13 (0) has triggered the IT */
  {
    EXTI->PR |= EXTI_PR_PR13; /* Clear the pending bit */
	
		LED_TOGGLE;
		
		state_run = 1;
  }
  else /* Should never occur */
  {
//    GPIOC->BSRR = (1<<8); /* Switch on orange led to report an error */
  }
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
