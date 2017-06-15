#include "i2c.h"


	/**
  * @brief  This function :
             - Enables GPIO clock
             - Configures the I2C2 pins on GPIO PB10 PB11
  * @param  None
  * @retval None
  */
  

  


	
void I2C_ConfigTimingPe(I2C_TypeDef* pI2C)
{
  /* Configure I2C2, master */
  /* (1) Timing register value is computed with the AN4235 xls file,
   fast Mode @400kHz with I2CCLK = 48MHz, rise time = 140ns, fall time = 40ns */
  /* (2) Periph enable */
  pI2C->TIMINGR = (uint32_t)0x00B01A4B; /* (1) */
  pI2C->CR1 = I2C_CR1_PE; /* (2) */


}

void I2C_ClearHandleBuffers(I2CStructHandle* pHI2C)
{
	clear_buffer(&(pHI2C->RxBuff));
	clear_buffer(&(pHI2C->TxBuff));
}


/*********************************************************************************/
/*                                                                               */
/*                                HAL VERSION                                    */
/*                                                                               */
/*********************************************************************************/

// Mode		:		I2C_AUTOEND_MODE, I2C_RELOAD_MODE
// Request	:		I2C_GENERATE_START_READ, I2C_GENERATE_START_WRITE
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
//

//

I2CStateEnum HAL_I2C_Master_Receive(I2CStructHandle* pHI2C)
{
	uint32_t timeout = 0;
	I2CStateEnum nack_state;
	
	/* Prepare transfer parameters */
	pHI2C->RxBuff.ix_ary = 0;

	/* Send Slave Address */
	/* Set NBYTES to write and reload if hi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART */
	I2C_TransferConfig(pHI2C->pI2C, pHI2C->devAddress, pHI2C->RxBuff.transfer_size, I2C_AUTOEND_MODE, I2C_GENERATE_START_READ);

	/* Wait until RXNE flag is set */
	while((pHI2C->pI2C->ISR & I2C_ISR_RXNE) == 0U)
	{
		/* Check only one first time if a NACK is detected */
		nack_state = I2C_IsAcknowledgeFailed(pHI2C->pI2C);
	
		if(nack_state == I2C_STATE_NOT_NACK)
		{
			/* Check for the Timeout */
			if(timeout < TIMEOUT_RXNE) timeout++;
			else return I2C_STATE_TIMEOUT_RXNE; 
		}
		else return nack_state; // is error state: I2C_STATE_TIMEOUT_NACK_STOP, I2C_STATE_NACK_STOP
	}

	/* Read data from RXDR */
	pHI2C->RxBuff.p_ary_data[pHI2C->RxBuff.ix_ary++] = pHI2C->pI2C->RXDR;
	pHI2C->RxBuff.transfer_size--;
	
	
	
	while(pHI2C->RxBuff.transfer_size > 0U)
	{
		timeout = 0;
		
		/* Wait until RXNE flag is set */
		while((pHI2C->pI2C->ISR & I2C_ISR_RXNE) == 0U)
		{
			/* Check for the Timeout */
			if(timeout < TIMEOUT_RXNE) timeout++;
			else return I2C_STATE_TIMEOUT_RXNE; 
		}

		/* Read data from RXDR */
		pHI2C->RxBuff.p_ary_data[pHI2C->RxBuff.ix_ary++] = pHI2C->pI2C->RXDR;
		pHI2C->RxBuff.transfer_size--;
	}

	timeout = 0;
	
	/* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
	/* Wait until STOPF flag is set */
	while((pHI2C->pI2C->ISR & I2C_ISR_STOPF) == 0U) 
	{
		if(timeout < TIMEOUT_AUTOEND_STOP)timeout++;
		else return I2C_STATE_TIMEOUT_AUTOEND_STOP;
	}

	/* Clear STOP Flag */
	pHI2C->pI2C->ICR|=I2C_ICR_STOPCF;

	/* Clear Configuration Register 2 */
	pHI2C->pI2C->CR2 &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN));

	return I2C_STATE_TRANSFER_DONE;
}
//

I2CStateEnum HAL_I2C_Master_Transmit(I2CStructHandle* pHI2C)
{
	uint32_t timeout;
	I2CStateEnum nack_state;
	pHI2C->TxBuff.ix_ary = 0;
	
	I2C_TransferConfig(pHI2C->pI2C, pHI2C->devAddress, pHI2C->TxBuff.transfer_size, I2C_AUTOEND_MODE, I2C_GENERATE_START_WRITE);

	timeout = 0;

	while(pHI2C->TxBuff.transfer_size > 0U)
	{
		/* Wait until TXIS flag is set */
		while((pHI2C->pI2C->ISR & I2C_ISR_TXIS) == 0U)
		{
			/* Check if a NACK is detected */
			nack_state = I2C_IsAcknowledgeFailed(pHI2C->pI2C);
			
			if(nack_state == I2C_STATE_NOT_NACK)
			{
				/* Check for the Timeout */
				/* Check for the Timeout */
				if(timeout < TIMEOUT_TXIS) timeout++;
				else return I2C_STATE_TIMEOUT_TXIS;
			}
			else return nack_state;  // is error state: I2C_STATE_TIMEOUT_NACK_STOP, I2C_STATE_NACK_STOP
		}
		
		/* Write data to TXDR */
		pHI2C->pI2C->TXDR	= pHI2C->TxBuff.p_ary_data[pHI2C->TxBuff.ix_ary++];
		pHI2C->TxBuff.transfer_size--;
	}

	timeout = 0;
	
	/* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
	/* Wait until STOPF flag is set */
	while((pHI2C->pI2C->ISR & I2C_ISR_STOPF) == 0U) 
	{
		/* Check if a NACK is detected */
			nack_state = I2C_IsAcknowledgeFailed(pHI2C->pI2C);
			
			if(nack_state == I2C_STATE_NOT_NACK)
			{
				/* Check for the Timeout */
				if(timeout < TIMEOUT_AUTOEND_STOP)timeout++;
				else return I2C_STATE_TIMEOUT_AUTOEND_STOP;
			}
			else return nack_state;  // is error state: I2C_STATE_TIMEOUT_NACK_STOP, I2C_STATE_NACK_STOP
	}

	/* Clear STOP Flag */
	pHI2C->pI2C->ICR|=I2C_ICR_STOPCF;

	/* Clear Configuration Register 2 */
	pHI2C->pI2C->CR2 &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN));

	return I2C_STATE_TRANSFER_DONE;
}
//

I2CStateEnum I2C_EEPROM_SetMemAddress(I2CStructHandle* pHI2C)
{
	uint32_t timeout;
	I2CStateEnum nack_state;
	pHI2C->TxBuff.ix_ary = 0;
	
	// send start and address
	I2C_TransferConfig(pHI2C->pI2C, pHI2C->devAddress, 1, I2C_SOFTEND_MODE, I2C_GENERATE_START_WRITE);
	
	timeout = 0;
	
	/* Wait until TXIS flag is set */
	while((pHI2C->pI2C->ISR & I2C_ISR_TXIS) == 0U)
	{
		/* Check if a NACK is detected */
		nack_state = I2C_IsAcknowledgeFailed(pHI2C->pI2C);
		
		if(nack_state == I2C_STATE_NOT_NACK)
		{
			/* Check for the Timeout */
			/* Check for the Timeout */
			if(timeout < TIMEOUT_TXIS) timeout++;
			else return I2C_STATE_TIMEOUT_TXIS;
		}
		else return nack_state;  // is error state: I2C_STATE_TIMEOUT_NACK_STOP, I2C_STATE_NACK_STOP
	}
	
	/* Write memory address to TXDR */
	pHI2C->pI2C->TXDR = pHI2C->TxBuff.p_ary_data[0];
	
	timeout = 0;
	
	/* Need to check TC flag, while SOFTEND mode is set */
	/* Wait until TC flag is set */
	while((pHI2C->pI2C->ISR & I2C_ISR_TC) == 0U) 
	{
		/* Check if a NACK is detected */
		nack_state = I2C_IsAcknowledgeFailed(pHI2C->pI2C);
		
		if(nack_state == I2C_STATE_NOT_NACK)
		{
			/* Check for the Timeout */
			if(timeout < TIMEOUT_AUTOEND_STOP)timeout++;
			else return I2C_STATE_TIMEOUT_AUTOEND_STOP;
		}
		else return nack_state;  // is error state: I2C_STATE_TIMEOUT_NACK_STOP, I2C_STATE_NACK_STOP
	}
	
	// TC is set, memory address pointer is set
	return I2C_STATE_TRANSFER_DONE;


}












