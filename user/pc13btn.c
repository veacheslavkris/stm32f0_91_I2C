#include "pc13btn.h"

#define BTN_C13_PIN_D_POS 			13

void BtnPc13_Init(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
	
	RCC->APB2ENR|=RCC_APB2ENR_SYSCFGEN;
	
	GpioSetInputModeInterrupt(GPIOC, EXTI_PC, BTN_C13_PIN_D_POS, EDGE_FALLING_INTERRUPT);

	NVIC_EnableIRQ(EXTI4_15_IRQn); /* (6) */
	NVIC_SetPriority(EXTI4_15_IRQn,0); /* (7) */
}







