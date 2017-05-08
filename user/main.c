/**
  ******************************************************************************
  * @file    01_HWInterruptSelection/main.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   This code example shows how to configure a GPIO to use it
  *          as external interrupt
  *
 ===============================================================================
                    #####       MCU Resources     #####
 ===============================================================================
   - PA0, PC9

 ===============================================================================
                    ##### How to use this example #####
 ===============================================================================
    - this file must be inserted in a project containing  the following files :
      o system_stm32f0xx.c, startup_stm32f072xb.s
      o stm32f0xx.h to get the register definitions
      o CMSIS files
    

 ===============================================================================
                    ##### How to test this example #####
 ===============================================================================
    - push the User button, this toggles the green led

  *    
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/


#include "main.h"

/** @addtogroup STM32F0_Snippets
  * @{
  */

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
	
	init_led_gpio();
  
	I2C2GPIOConfigure();

	I2C2MasterConfigure(I2C2);
	
	ConfigureExternalIT();

	/* MAX 7219 */ 
	start_max7219();
	
	delay_systick(3000);
	
	SetDigitSegmentMax7219(ADDR_DIG_0, DIGIT_0);
	SetDigitSegmentMax7219(ADDR_DIG_4, DIGIT_4);
	
  while (1) /* Infinite loop */
  {
		if(state_run == 1)
		{

			cur_temp = 0;
			
			cur_temp = I2C2MasterSendStartGetTempAutoEnd(I2C2, TMP275_ADDRESS);
			
			state_run = 0;
		}
  }

}

void delay_systick(uint32_t ms)
{
	systick_count = ms;
	
	while(systick_count !=0) continue;

}

void start_max7219()
{
	init_max7219_gpio();
	
	InitMax7219();

	ClearAllDigitsMax7219();
}


void init_led_gpio(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; /* (1) */  
	
	GpioSetModeOutputStrong(GPIOA, LED_GREEN_A5_D_POS, OSPEEDR_MEDIUM);
}

void ConfigureExternalIT(void)
{
	  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
		GpioSetInterruptMode(EXTI_PC, BTN_C13_PIN_POS);

		NVIC_EnableIRQ(EXTI4_15_IRQn); /* (6) */
		NVIC_SetPriority(EXTI4_15_IRQn,0); /* (7) */
}

void init_max7219_gpio(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 

	GpioSetModeOutputStrong(GPIOC, PIN_CLK_PC0_D_POS, OSPEEDR_MEDIUM);
	GpioSetModeOutputStrong(GPIOC, PIN_DOUT_PC1_D_POS, OSPEEDR_MEDIUM);
	GpioSetModeOutputStrong(GPIOC, PIN_LATCH_PC3_D_POS, OSPEEDR_MEDIUM);
}

void LatchMax7219Off(void)
{
	GPIOC->BRR = GPIO_BRR_BR_3;
}

void LatchMax7219On(void)
{
	GPIOC->BSRR = GPIO_BSRR_BS_3;
}

void ClkMax7219Off(void)
{
	GPIOC->BRR = GPIO_BRR_BR_0;
}

void ClkMax7219On(void)
{
	GPIOC->BSRR = GPIO_BSRR_BS_0;
}

void SetDataPin(uint32_t val)
{
	if(val) GPIOC->BSRR = GPIO_BSRR_BS_1;
	else GPIOC->BRR = GPIO_BRR_BR_1;
}

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
	if(systick_count > 0) systick_count--;
	
}

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
//    GPIOA->ODR ^= (1<<5); /* Toggle green led on PA5 */
//		GPIOA->ODR ^= GPIO_ODR_5;
		GPIOA->ODR ^= LED_GREEN_A5_B_POS;
		
		state_run = 1;
  }
  else /* Should never occur */
  {
//    GPIOC->BSRR = (1<<8); /* Switch on orange led to report an error */
  }
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
