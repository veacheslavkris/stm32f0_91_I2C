#ifndef TMP275_H
#define TMP275_H

#include "i2c.h"
#include "gpio.h"

#define ALT_FUNC		ALT_FUNC_1
#define PIN_SCL			10
#define PIN_SDA			11
#define PORT_I2C		GPIOB

#define I2C					I2C2



typedef enum
{
	MEASUREMENT_SUCCESSFULL,
	MEASUREMENT_FAIL
}
TMP275ExecutionEnum;

typedef struct
{
	float temprt;
	TMP275ExecutionEnum execState;

}
TMP275ExecutionResult;




void TMP275_Init(void);

__STATIC_INLINE void i2c_init(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	
  /* (1) Open drain for I2C signals */
  /* (2) AF1 for I2C signals */
  /* (3) Select AF mode (10) on PB10 and PB11 */
	
	GpioSetModeI2C(PORT_I2C, PIN_SCL, PIN_SDA, ALT_FUNC);
	
		/* Enable the peripheral clock I2C2 */
  RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	I2C_ConfigTimingPe(I2C);
}
//

I2CStructHandle* TMP275_GetHandle(void);

//void TMP275_ClearBuffers(void);



TMP275ExecutionResult* TMP275GetTemperature(void);






















#endif













