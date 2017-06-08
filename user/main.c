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
	
	I2C2_Init();
	
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
			cur_temp = 0;
			
			cur_temp = I2C_MasterStartGetTempAutoEnd(I2C2, TMP275_ADDRESS);
			structFloatToBcd.flt = cur_temp;
			
			ConvertFloatToBcd(&structFloatToBcd);
			
			DisplayMax7219_TT_tt(0, &structFloatToBcd);
			
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
