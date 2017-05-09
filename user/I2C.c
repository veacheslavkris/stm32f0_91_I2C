#include "I2C.h"


	/**
  * @brief  This function :
             - Enables GPIO clock
             - Configures the I2C2 pins on GPIO PB10 PB11
  * @param  None
  * @retval None
  */
	
	
//void I2C2GPIOConfigure(void)
//{
//  /* Enable the peripheral clock of GPIOB */
//  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
//	
//  /* (1) Open drain for I2C signals */
//  /* (2) AF1 for I2C signals */
//  /* (3) Select AF mode (10) on PB10 and PB11 */
//	
//	GpioSetModeI2C(GPIOB, 10, ALT_FUNC_1);
//	GpioSetModeI2C(GPIOB, 11, ALT_FUNC_1);

//	
//}

void I2C2ConfigMstrTimingPe(I2C_TypeDef* pI2C)
{
  /* Configure I2C2, master */
  /* (1) Timing register value is computed with the AN4235 xls file,
   fast Mode @400kHz with I2CCLK = 48MHz, rise time = 140ns, fall time = 40ns */
  /* (2) Periph enable */
  pI2C->TIMINGR = (uint32_t)0x00B01A4B; /* (1) */
  pI2C->CR1 = I2C_CR1_PE; /* (2) */


}

uint32_t I2C2MasterSendStartOneByteAutoEnd(I2C_TypeDef* pI2C, uint32_t slaveAddress, uint32_t data_byte)
{
	// set: autoend, slave address
	pI2C->CR2 |=  I2C_CR2_AUTOEND|(((uint8_t)slaveAddress)<<1);

	// send write state
	pI2C->CR2 &= ~(I2C_CR2_RD_WRN);
	
	// set count 1 byte
	pI2C->CR2 |= 1<<16;

	

	pI2C->CR2|=I2C_CR2_START; // Go
	
	/* Wait until TXIS flag is set */
	
	/* - 1. Check if a NACK is detected */
	/* - 2. Check for the Timeout */
	while((pI2C->ISR & I2C_ISR_TXIS) == I2C_ISR_TXIS) continue;
	
	pI2C->TXDR = (uint8_t)data_byte; /* Byte to send */
	
	/* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
  /* Wait until STOPF flag is set */
	while((pI2C->ISR & I2C_ISR_STOPF) != I2C_ISR_STOPF) continue;
	pI2C->ISR&=~(I2C_ICR_STOPCF);
	
	return 0;


}

float I2C2MasterSendStartGetTempAutoEnd(I2C_TypeDef* pI2C, uint32_t slaveAddress)
{
	uint8_t temp_1 = 0x00;
	uint8_t temp_2 = 0x00;
	uint32_t temp_16 = 0;
	float temp_f1 = 0.0;
	uint32_t cnt = 0;
	
	// set: autoend, read state, slave address
	pI2C->CR2 |=  I2C_CR2_AUTOEND|I2C_CR2_RD_WRN|(((uint8_t)slaveAddress)<<1);

	// set count 2 bytes
	pI2C->CR2 |= 2<<16;
	
	pI2C->CR2|=I2C_CR2_START; // Go
	
	while(cnt<2)
	{
		/* while wait for I2C_ISR_RXNE */
		
		/*	- 1. Check if a NACK is detected		*/
		/*	- 2. Check if a STOPF is detected		*/
		/*	- 3. Check for the Timeout					*/
		
		if((pI2C->ISR & I2C_ISR_RXNE) == I2C_ISR_RXNE)
		{
			/* Read receive register, will clear RXNE flag */
			if(cnt == 0) temp_1 = pI2C->RXDR;
			else if(cnt == 1) temp_2 = pI2C->RXDR;
			
			cnt++;
		}
	}
	
	while((pI2C->ISR & I2C_ISR_STOPF) != I2C_ISR_STOPF) continue;
		
	pI2C->ISR&=~(I2C_ICR_STOPCF);
		
	temp_16 = temp_1<<8;
	temp_16 = temp_16|temp_2;
	temp_16 = temp_16 >> 4;
	
	temp_f1 = ((float)(temp_16/4)*(float)0.24); 
	
	return temp_f1;
	
}



























