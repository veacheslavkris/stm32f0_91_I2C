#include "hardware.h"




void init_rcc(void)
{
	
	// I2C
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	
}
//

void HWInitGpio()
{
	init_rcc();
	
//	init_led_gpio();
	init_I2C_gpio();

	
	
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








