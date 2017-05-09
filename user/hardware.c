#include "hardware.h"

void init_rcc(void)
{
	// led gpio
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; /* (1) */ 

	// I2C, MAXIM7219
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	
	// MAXIM7219, BTN_C13, UART
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
}
//

void HWInitGpio()
{
	init_rcc();
	
	init_led_gpio();
	init_I2C_gpio();
	init_max7219_gpio();
	init_btn_interrupt();
	init_uart8_gpio();
}
//

void init_led_gpio(void)
{
	GpioSetModeOutputStrong(GPIOA, LED_GREEN_A5_D_POS, OSPEEDR_MEDIUM);
}
//

void init_I2C_gpio(void)
{
  /* (1) Open drain for I2C signals */
  /* (2) AF1 for I2C signals */
  /* (3) Select AF mode (10) on PB10 and PB11 */
	
	GpioSetModeI2C(GPIOB, 10, ALT_FUNC_1);
	GpioSetModeI2C(GPIOB, 11, ALT_FUNC_1);
}
//

void init_max7219_gpio(void)
{
	GpioSetModeOutputStrong(GPIOC, PIN_CLK_PC0_D_POS, OSPEEDR_MEDIUM);
	GpioSetModeOutputStrong(GPIOC, PIN_DOUT_PC1_D_POS, OSPEEDR_MEDIUM);
	GpioSetModeOutputStrong(GPIOB, PIN_LATCH_PB0_D_POS, OSPEEDR_MEDIUM);
}
//

void init_btn_interrupt(void)
{
//		SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
	  RCC->APB2ENR|=RCC_APB2ENR_SYSCFGEN;
		
		GpioSetInterruptMode(EXTI_PC, BTN_C13_PIN_POS, EDGE_RISING_FALLING_ITR);

		NVIC_EnableIRQ(EXTI4_15_IRQn); /* (6) */
		NVIC_SetPriority(EXTI4_15_IRQn,0); /* (7) */
}
//

void init_uart8_gpio(void)
{
	//  /* (1) Select AF mode (10) on PC2 and PC3 */
	//  /* (2) AF2 for USART8 signals */
	GpioSetModeUart(GPIOC, 2, 3, ALT_FUNC_2);
//	GpioSetModeUart(GPIOC, 3, ALT_FUNC_2);
}
//






