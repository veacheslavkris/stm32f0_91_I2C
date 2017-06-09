#include "a_i2c.h"


	/**
  * @brief  This function :
             - Enables GPIO clock
             - Configures the I2C2 pins on GPIO PB10 PB11
  * @param  None
  * @retval None
  */
	
	
void I2C_ConfigMstrTimingPe(I2C_TypeDef* pI2C)
{
  /* Configure I2C2, master */
  /* (1) Timing register value is computed with the AN4235 xls file,
   fast Mode @400kHz with I2CCLK = 48MHz, rise time = 140ns, fall time = 40ns */
  /* (2) Periph enable */
  pI2C->TIMINGR = (uint32_t)0x00B01A4B; /* (1) */
  pI2C->CR1 = I2C_CR1_PE; /* (2) */


}

uint32_t I2C_MasterStartSendOneByteAutoEnd(I2C_TypeDef* pI2C, uint32_t slaveAddress, uint32_t data_byte)
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

float I2C_MasterStartGetTempAutoEnd(I2C_TypeDef* pI2C, uint32_t slaveAddress)
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
		
	temp_16 = (temp_1<<4)|(temp_2>>4);
	
	temp_f1 = temp_16/16.0f;
	
	return temp_f1;
	
}

/*********************************************************************************/
/*                                                                               */
/*                                HAL VERSION                                    */
/*                                                                               */
/*********************************************************************************/

// Mode			:		I2C_AUTOEND_MODE, I2C_RELOAD_MODE
// Request	:		I2C_GENERATE_START_READ, I2C_GENERATE_START_WRITE
// I2CStructHandle
void I2C_TransferConfig(I2C_TypeDef* pI2C,  uint16_t DevAddress, uint8_t Size, uint32_t Mode, uint32_t Request)
{
  uint32_t tmpreg = 0U;
	
	DevAddress = DevAddress << 1;

  /* Check the parameters */
 
  /* Get the CR2 register value */
  tmpreg = pI2C->CR2;

  /* clear tmpreg specific bits */
  tmpreg &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));

  /* update tmpreg */
  tmpreg |= (uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << 16 ) & I2C_CR2_NBYTES) | (uint32_t)Mode | (uint32_t)Request);

  /* update CR2 register */
  pI2C->CR2 = tmpreg;
}



// void HAL_I2C_Master_Receive(I2C_TypeDef* pI2C, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
void HAL_I2C_Master_Receive(I2CStructHandle* pHI2C)
{
	/* Prepare transfer parameters */
	pHI2C->RxBuff.ix_ary = 0;
	
	
	/* Send Slave Address */
	/* Set NBYTES to write and reload if hi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
	I2C_TransferConfig(pHI2C->pI2C, pHI2C->devAddress, pHI2C->RxBuff.transfer_size, I2C_AUTOEND_MODE, I2C_GENERATE_START_READ);

	while(pHI2C->RxBuff.transfer_size > 0U)
	{
		/* Wait until RXNE flag is set */
		if(!I2C_WaitOnRXNEFlagUntilTimeout)
		{
			return HAL_ERROR / return HAL_TIMEOUT;;
		}

		/* Read data from RXDR */
		pHI2C->RxBuff.ary_data[pHI2C->RxBuff.ix_ary++] = pHI2C->pI2C->RXDR;
		pHI2C->RxBuff.transfer_size--;

	}

	/* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
	/* Wait until STOPF flag is set */
//    if(I2C_WaitOnSTOPFlagUntilTimeout)return HAL_ERROR / return HAL_TIMEOUT;
	while((pHI2C->pI2C->ISR & I2C_ISR_STOPF) == 0U) continue;
    
	/* Clear STOP Flag */
	pHI2C->pI2C->ICR|=I2C_ICR_STOPCF;
	
	/* Clear Configuration Register 2 */
	pHI2C->pI2C->CR2 &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN));

	
	return HAL_OK;

}



//static HAL_StatusTypeDef I2C_IsAcknowledgeFailed(I2C_TypeDef* pI2C, uint32_t Timeout, uint32_t Tickstart)
//{
////  if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF) == SET)
//  if((pI2C->ISR & I2C_ISR_NACKF) == I2C_ISR_NACKF)
//  {
//    /* Wait until STOP Flag is reset */
//    /* AutoEnd should be initiate after AF */
////    while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == RESET)
//    while((pI2C->ISR & I2C_ISR_STOPF) == 0)
//    {
//      /* Check for the Timeout */
//      if(Timeout != HAL_MAX_DELAY)
//      {
//      if((Timeout == 0U)||((HAL_GetTick() - Tickstart) > Timeout))
//        {
//          hi2c->State= HAL_I2C_STATE_READY;
//          hi2c->Mode = HAL_I2C_MODE_NONE;

//          /* Process Unlocked */
//          __HAL_UNLOCK(hi2c);
//          return HAL_TIMEOUT;
//        }
//      }
//    }

//    /* Clear NACKF Flag */
//    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

//    /* Clear STOP Flag */
//    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

//    /* Flush TX register */
//    I2C_Flush_TXDR(hi2c);

//    /* Clear Configuration Register 2 */
//    I2C_RESET_CR2(hi2c);

//    hi2c->ErrorCode = HAL_I2C_ERROR_AF;
//    hi2c->State= HAL_I2C_STATE_READY;
//    hi2c->Mode = HAL_I2C_MODE_NONE;

//    /* Process Unlocked */
//    __HAL_UNLOCK(hi2c);

//    return HAL_ERROR;
//  }
//  return HAL_OK;
//}

//static HAL_StatusTypeDef I2C_WaitOnRXNEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)
//{
//  while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == RESET)
//  {
//    /* Check if a NACK is detected */
//    if(I2C_IsAcknowledgeFailed(hi2c, Timeout, Tickstart) != HAL_OK)
//    {
//      return HAL_ERROR;
//    }

//    /* Check if a STOPF is detected */
//    if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == SET)
//    {
//      /* Clear STOP Flag */
//      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

//      /* Clear Configuration Register 2 */
//      I2C_RESET_CR2(hi2c);

//      hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
//      hi2c->State= HAL_I2C_STATE_READY;
//      hi2c->Mode = HAL_I2C_MODE_NONE;

//      /* Process Unlocked */
//      __HAL_UNLOCK(hi2c);

//      return HAL_ERROR;
//    }

//    /* Check for the Timeout */
//    if((Timeout == 0U)||((HAL_GetTick() - Tickstart) > Timeout))
//    {
//      hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
//      hi2c->State= HAL_I2C_STATE_READY;

//      /* Process Unlocked */
//      __HAL_UNLOCK(hi2c);

//      return HAL_TIMEOUT;
//    }
//  }
//  return HAL_OK;
//}

























