#ifndef EEPROM_H
#define EEPROM_H

#include "i2c.h"
#include "gpio.h"

#define ALT_FUNC		ALT_FUNC_1
#define PIN_SCL			10
#define PIN_SDA			11
#define PORT_I2C		GPIOB

#define I2C				I2C2



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

void EEPROM_2Kb_Init(void);

I2CStateEnum EEPROM_Read(uint32_t start_pos, uint32_t count);
I2CStateEnum EEPROM_Write(uint32_t start_pos, uint32_t count_for_write);

uint8_t* EEPROM_GetRxBufferArray(void);
uint8_t* EEPROM_GetTxBufferArray(void);

void EEPROM_ClearRxTxBuffers(void);
I2CStructHandle* EEPROM_GetHandle(void);


#endif






