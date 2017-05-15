/**
  ******************************************************************************
  * @file    01_HWInterruptSelection/main.c 
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
	uint32_t systick_count =0;
	
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

	RtcInitLse();
	
	HWInitGpio();
	
	/* Enable the peripheral clock I2C2 */
  RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	I2C2ConfigMstrTimingPe(I2C2);
	
//	UartSendString(USART8, "Hello", 5, CR_ON);
	
	
	/* MAX 7219 */ 
	Max7219_Init();

	Max7219_ClearAllDigits();
	
	delay_systick(1000);
	
	Max7219_ShowAtPositionNumber(11, 0);

	delay_systick(2000);
	Max7219_ShowAtPositionNumber(0, 15);
	delay_systick(2000);

	Max7219_ClearAllDigits();

	Max7219_ShowAtPositionNumber(0,0);
	Max7219_ShowAtPositionNumber(4,0);


//	UartSendCharCR(USART8, 'A');

  BtnPc13_Init();
	
	LedPA5_Init();
	
	LED_ON;
	delay_systick(2000);
	LED_OFF;	
	
	while (1) /* Infinite loop */
  {
		if(state_run == 1)
		{
			cur_temp = 0;
			
			cur_temp = I2C2MasterSendStartGetTempAutoEnd(I2C2, TMP275_ADDRESS);
			
			state_run = 0;
		}
		
		cycle_digits();
		delay_systick(1000);
  }

}

void delay_systick(uint32_t ms)
{
	systick_count = ms;
	
	while(systick_count !=0) continue;

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
void SysTick_Handler(void)
{
	if(systick_count > 0) systick_count--;
	
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
