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
	I2C_TypeDef* pI2C = pHI2C->pI2C;
	
	/* Prepare transfer parameters */
	pHI2C->RxBuff.ix_ary = 0;

	/* Send Slave Address */
	I2C_TransferConfig(pI2C, pHI2C->devAddress, pHI2C->RxBuff.transfer_size, I2C_AUTOEND_MODE, I2C_GENERATE_START_READ);

	timeout = TIMEOUT_RXNE;
	
	/* Wait until RXNE flag is set */
	while(IS_RXNE_CLEAR)
	{
		/* Check only one first time if a NACK is detected */
		nack_state = I2C_IsAcknowledgeFailed(pHI2C->pI2C);
	
		if(nack_state == I2C_STATE_NOT_NACK)
		{
			if(IS_END_TIMEOUT_DEC) return I2C_STATE_TIMEOUT_RXNE; 
		}
		else return nack_state; // is error state: I2C_STATE_TIMEOUT_NACK_STOP, I2C_STATE_NACK_STOP
	}

	/* Read data from RXDR */
	pHI2C->RxBuff.p_ary_data[pHI2C->RxBuff.ix_ary++] = pHI2C->pI2C->RXDR;
	pHI2C->RxBuff.transfer_size--;
		
	while(pHI2C->RxBuff.transfer_size > 0U)
	{
		timeout = TIMEOUT_RXNE;
		
		/* Wait until RXNE flag is set */
		while(IS_RXNE_CLEAR)	
		{
			if(IS_END_TIMEOUT_DEC) return I2C_STATE_TIMEOUT_RXNE; 
		}

		/* Read data from RXDR */
		pHI2C->RxBuff.p_ary_data[pHI2C->RxBuff.ix_ary++] = pHI2C->pI2C->RXDR;
		pHI2C->RxBuff.transfer_size--;
	}

	timeout = TIMEOUT_AUTOEND_STOP;
	
	/* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
	/* Wait until STOPF flag is set */
	while(IS_STOPF_CLEAR)	
	{
		if(IS_END_TIMEOUT_DEC)return I2C_STATE_TIMEOUT_AUTOEND_STOP;
	}

	/* Clear STOP Flag */
	CLEAR_STOP_FLAG;
	
	/* Clear Configuration Register 2 */
	CLEAR_CR2;
	
	return I2C_STATE_TRANSFER_DONE;
}
//
//		while((IS_TXIS_CLEAR) && (--timeout > 0U))
I2CStateEnum HAL_I2C_Master_Transmit(I2CStructHandle* pHI2C)
{
	I2C_TypeDef* pI2C = pHI2C->pI2C;
	uint32_t cnt_bytes = pHI2C->TxBuff.transfer_size; // including 2 bytes of start mem position
	uint8_t* p_tx_data = pHI2C->TxBuff.p_ary_data;
	volatile uint32_t timeout = TIMEOUT_TXIS;
	
	I2C_TransferConfig(pHI2C->pI2C, pHI2C->devAddress, pHI2C->TxBuff.transfer_size, I2C_AUTOEND_MODE, I2C_GENERATE_START_WRITE);

	// transfer bytes_mem_address and then data
	while(cnt_bytes > 0U)
	{
		/* Wait until TXIS flag is set */ 
		while((IS_TXIS_CLEAR) && (HAS_TIMEOUT_TICKS))	
		{
			/* Check if a NACK is detected */
			if(IS_NACKF_SET) return nack_process(pI2C);
		}

		if(IS_TIMEOUT_ZERO) return I2C_STATE_TIMEOUT_TXIS;
		else // TXIS IS SET
		{
			/* Write data to TXDR */
			pI2C->TXDR	= (*p_tx_data++);
			cnt_bytes--;
		}
	}

	timeout = TIMEOUT_AUTOEND_STOP;
	
	/* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
	/* Wait until STOPF flag is set */
	while((IS_STOPF_CLEAR) && (HAS_TIMEOUT_TICKS))	
	{
		/* Check if a NACK is detected */
		if(IS_NACKF_SET) return nack_process(pI2C);	
	}

	if(IS_TIMEOUT_ZERO) return I2C_STATE_TIMEOUT_AUTOEND_STOP;
	else // STOPF is set in time
	{
		/* Clear STOP Flag */
		CLEAR_STOP_FLAG;

		/* Clear Configuration Register 2 */
		CLEAR_CR2;
		
		return I2C_STATE_TRANSFER_DONE;
	}
	

	

}
//

I2CStateEnum HAL_I2C_Master_Transmit_2(I2CStructHandle* pHI2C)
{
	uint32_t timeout;
	I2CStateEnum nack_state;
	I2C_TypeDef* pI2C = pHI2C->pI2C;
	
	pHI2C->TxBuff.ix_ary = 0;
		
	I2C_TransferConfig(pHI2C->pI2C, pHI2C->devAddress, pHI2C->TxBuff.transfer_size, I2C_AUTOEND_MODE, I2C_GENERATE_START_WRITE);

	timeout = TIMEOUT_TXIS;

	while(pHI2C->TxBuff.transfer_size > 0U)
	{
		/* Wait until TXIS flag is set */
		while(IS_TXIS_CLEAR)	
		{
			/* Check if a NACK is detected */
			nack_state = I2C_IsAcknowledgeFailed(pHI2C->pI2C);
			
			if(nack_state == I2C_STATE_NOT_NACK)
			{
					if(IS_END_TIMEOUT_DEC) return I2C_STATE_TIMEOUT_TXIS;
			}
			else
			{
				return nack_state;  // is error state: I2C_STATE_TIMEOUT_NACK_STOP, I2C_STATE_NACK_STOP
			}
		}
		
		/* Write data to TXDR */
		pHI2C->pI2C->TXDR	= pHI2C->TxBuff.p_ary_data[pHI2C->TxBuff.ix_ary++];
		pHI2C->TxBuff.transfer_size--;
	}

	timeout = TIMEOUT_AUTOEND_STOP;
	
	/* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
	/* Wait until STOPF flag is set */
	while(IS_STOPF_CLEAR)
	{
		/* Check if a NACK is detected */
			nack_state = I2C_IsAcknowledgeFailed(pHI2C->pI2C);
			
			if(nack_state == I2C_STATE_NOT_NACK)
			{
				if(IS_END_TIMEOUT_DEC) return I2C_STATE_TIMEOUT_AUTOEND_STOP;
			}
			else return nack_state;  // is error state: I2C_STATE_TIMEOUT_NACK_STOP, I2C_STATE_NACK_STOP
	}

	/* Clear STOP Flag */
	CLEAR_STOP_FLAG;

	/* Clear Configuration Register 2 */
	CLEAR_CR2;
	
	return I2C_STATE_TRANSFER_DONE;
}
//

I2CStateEnum I2C_EEPROM_SetMemAddress(I2C_TypeDef* pI2C, uint32_t i2c_address, uint32_t cnt_bytes_mem_address, uint8_t* p_mem_address)
{
	uint32_t timeout;
	
	// send start and address
	I2C_TransferConfig(pI2C, i2c_address, cnt_bytes_mem_address, I2C_SOFTEND_MODE, I2C_GENERATE_START_WRITE);
	
	timeout = TIMEOUT_TXIS;
	
	// transfer bytes_mem_address
	while(cnt_bytes_mem_address > 0U)
	{
		/* Wait until TXIS flag is set */
		while((IS_TXIS_CLEAR) && (HAS_TIMEOUT_TICKS))	
		{
			/* Check if a NACK is detected */
			if(IS_NACKF_SET) return nack_process(pI2C);
		}

		if(IS_TIMEOUT_ZERO) return I2C_STATE_TIMEOUT_TXIS;
		else // TXIS IS SET
		{
			/* Write data to TXDR */
			pI2C->TXDR	= (*p_mem_address++);
			cnt_bytes_mem_address--;
		}
	}
	
	// one or two bytes of mem address is transfered
	
	timeout = TIMEOUT_TC;
	
	/* Need to check TC flag, while SOFTEND mode is set */
	/* Wait until TC flag is set */
	while((IS_TC_CLEAR) && (HAS_TIMEOUT_TICKS))		
	{
		/* Check if a NACK is detected */
		if(IS_NACKF_SET) return nack_process(pI2C);
	}
	
	if(IS_TIMEOUT_ZERO) return I2C_STATE_TIMEOUT_TC;
	else return I2C_STATE_MEM_ADR_TRANSFER_DONE; 
}
//

I2CStateEnum I2C_EEPROM_SetMemAddress_2(I2C_TypeDef* pI2C, uint32_t i2c_address, uint32_t cnt_bytes_mem_address, uint8_t* p_mem_address)
{
	I2CStateEnum state;
	
	// send start and address
	I2C_TransferConfig(pI2C, i2c_address, cnt_bytes_mem_address, I2C_SOFTEND_MODE, I2C_GENERATE_START_WRITE);
	
	// transfer bytes_mem_address
	while(cnt_bytes_mem_address > 0U)
	{
		/* Wait until TXIS flag is set */
		
		//return: I2C_STATE_TIMEOUT_FLAG, I2C_STATE_FLAG_SET, I2C_STATE_TIMEOUT_NACK_STOP, I2C_STATE_NACK_STOP
		state =  wait_flag_by_nack_timeout(pI2C, I2C_ISR_TXIS, TIMEOUT_TXIS);
		
		if(state == I2C_STATE_FLAG_SET)
		{
			/* Write data to TXDR */
			pI2C->TXDR	= (*p_mem_address++);
			cnt_bytes_mem_address--;
		}
		else // ERRORS: I2C_STATE_TIMEOUT_FLAG, I2C_STATE_TIMEOUT_NACK_STOP, I2C_STATE_NACK_STOP
		{		
			if(state == I2C_STATE_TIMEOUT_FLAG) state = I2C_STATE_TIMEOUT_TXIS;
			
			return state;
		}
	}
	
	// one or two bytes of mem address is transfered
	
	/* Need to check TC flag, while SOFTEND mode is set */
	/* Wait until TC flag is set */
	
	//return: I2C_STATE_TIMEOUT_FLAG, I2C_STATE_FLAG_SET, I2C_STATE_TIMEOUT_NACK_STOP, I2C_STATE_NACK_STOP
	state =  wait_flag_by_nack_timeout(pI2C, I2C_ISR_TC, TIMEOUT_TC);
	
	if(state == I2C_STATE_TIMEOUT_FLAG) state = I2C_STATE_TIMEOUT_TC;
	else if(state == I2C_STATE_FLAG_SET) state = I2C_STATE_MEM_ADR_TRANSFER_DONE;
	
	return state;
	
	// next steps: restart by reading or writing
	


}




I2CStateEnum I2C_EEPROM_SetMemAddress_3(I2C_TypeDef* pI2C, uint32_t i2c_address, uint32_t cnt_bytes_mem_address, uint8_t* p_mem_address)
{
	uint32_t timeout;
	I2CStateEnum nack_state;
	
	// send start and address
	I2C_TransferConfig(pI2C, i2c_address, cnt_bytes_mem_address, I2C_SOFTEND_MODE, I2C_GENERATE_START_WRITE);
	
	timeout = TIMEOUT_TXIS;
	
	// transfer bytes_mem_address
	while(cnt_bytes_mem_address > 0U)
	{
		/* Wait until TXIS flag is set */
		while(IS_TXIS_CLEAR)	
		{
			/* Check if a NACK is detected */
			nack_state = I2C_IsAcknowledgeFailed(pI2C);
			
			if(nack_state == I2C_STATE_NOT_NACK)
			{
				if(IS_END_TIMEOUT_DEC) return I2C_STATE_TIMEOUT_TXIS;
			}
			else // NACK is detected
			{		
				return nack_state;  // is error state: I2C_STATE_TIMEOUT_NACK_STOP, I2C_STATE_NACK_STOP
			}
		}
		
		/* Write data to TXDR */
		pI2C->TXDR	= (*p_mem_address++);
		cnt_bytes_mem_address--;
	}
	
	// one or two bytes of mem address is transfered
	
	timeout = TIMEOUT_AUTOEND_STOP;
	
	/* Need to check TC flag, while SOFTEND mode is set */
	/* Wait until TC flag is set */
	while(IS_TC_CLEAR)		
	{
		/* Check if a NACK is detected */
		nack_state = I2C_IsAcknowledgeFailed(pI2C);
		
		if(nack_state == I2C_STATE_NOT_NACK)
		{
			if(IS_END_TIMEOUT_DEC) return I2C_STATE_TIMEOUT_TC;
		}
		else // NACK is detected
		{
			return nack_state;  // is error state: I2C_STATE_TIMEOUT_NACK_STOP, I2C_STATE_NACK_STOP
		}
	}
	
	// TC is set, memory address pointer is set
	// next steps: restart by reading or writing
	return I2C_STATE_MEM_ADR_TRANSFER_DONE;


}









