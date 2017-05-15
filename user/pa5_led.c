#include "pa5_led.h"



void LedPA5_Init(void)
{
	// led gpio
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; /* (1) */ 

	GpioSetModeOutputStrong(GPIOA, LED_GREEN_D_POS, OSPEEDR_MEDIUM);


}









