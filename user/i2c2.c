#include "i2c2.h"


#define ALT_FUNC		ALT_FUNC_1
#define PIN_SCL			10
#define PIN_SDA			11
#define PORT_I2C		GPIOB

#define I2C					I2C2


void I2C2_Init(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	
  /* (1) Open drain for I2C signals */
  /* (2) AF1 for I2C signals */
  /* (3) Select AF mode (10) on PB10 and PB11 */
	
	GpioSetModeI2C(PORT_I2C, PIN_SCL, PIN_SDA, ALT_FUNC);
	
		/* Enable the peripheral clock I2C2 */
  RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	I2C_ConfigMstrTimingPe(I2C);
}

























