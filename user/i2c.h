#ifndef I2C_H
	#define I2C_H

#include "stm32f091xc.h"
	
	
#define  I2C_GENERATE_STOP              I2C_CR2_STOP
#define  I2C_GENERATE_START_READ        (uint32_t)(I2C_CR2_START | I2C_CR2_RD_WRN)
#define  I2C_GENERATE_START_WRITE       I2C_CR2_START

#define  I2C_RELOAD_MODE                I2C_CR2_RELOAD
#define  I2C_AUTOEND_MODE               I2C_CR2_AUTOEND
#define  I2C_SOFTEND_MODE               ((uint32_t)0U)
	
	
/* I2C slave address to communicate with */
//#define TMP275_ADDRESS	(uint32_t)0x48

#define TRANSFER_DIR_WRITE	(uint32_t)0
#define TRANSFER_DIR_READ		(uint32_t)1

#define REG_TEMP_PTR     (uint32_t)0x00u
#define REG_CONF_PTR     (uint32_t)0x01u

#define TIMEOUT_AUTOEND_STOP	1000
#define TIMEOUT_RXNE			1000
#define TIMEOUT_TXIS			1000

#define TIMEOUT_NACK			1000
#define TIMEOUT_NACK_STOP		1000

#define TIMEOUT_I2C				1000

#define IS_TIMEOUT_DONE		(timeout-- == 0)

typedef enum
{
	I2C_STATE_TRANSFER_DONE,
	I2C_STATE_MEM_ADR_TRANSFER_DONE,
	I2C_STATE_TIMEOUT_AUTOEND_STOP,
	I2C_STATE_TIMEOUT_NACK_STOP,
	I2C_STATE_TIMEOUT_RXNE,
	I2C_STATE_TIMEOUT_TXIS,
	I2C_STATE_TIMEOUT_TC,
	I2C_STATE_NOT_NACK,
	I2C_STATE_NACK_STOP,
	I2C_STATE_RXNE,
	I2C_STATE_TXIS,
	I2C_STATE_STOP
	
}I2CStateEnum;

typedef struct
{
	uint32_t ary_size;
	uint32_t transfer_size;
	uint32_t ix_ary;
	uint8_t* p_ary_data;

}
I2CStructData;
//

typedef struct
{
	I2C_TypeDef* pI2C;
	I2CStructData RxBuff;
	I2CStructData TxBuff;
	uint32_t devAddress;

}
I2CStructHandle;




void I2C_ConfigTimingPe(I2C_TypeDef* pI2C);

void I2C_TransferConfig(I2C_TypeDef* pI2C,  uint16_t DevAddress, uint8_t Size, uint32_t Mode, uint32_t Request);

I2CStateEnum I2C_IsAcknowledgeFailed(I2C_TypeDef* pI2C);

I2CStateEnum HAL_I2C_Master_Receive(I2CStructHandle* pHI2C);

I2CStateEnum HAL_I2C_Master_Transmit(I2CStructHandle* pHI2C);

void I2C_ClearHandleBuffers(I2CStructHandle* pHI2C);

__STATIC_INLINE void clear_buffer(I2CStructData* pI2CStructData)
{
	uint32_t ix = 0;
	
	for(ix=0; ix< pI2CStructData->ary_size; ix++)
	{
		pI2CStructData->p_ary_data[ix]=0;
	}
	
	pI2CStructData->ix_ary = 0;
	pI2CStructData->transfer_size=0;
}
//

__STATIC_INLINE I2CStateEnum I2C_IsAcknowledgeFailed(I2C_TypeDef* pI2C)
{
//	uint32_t timeout = 0;
	uint32_t timeout = TIMEOUT_NACK_STOP;

	
	if((pI2C->ISR & I2C_ISR_NACKF) == I2C_ISR_NACKF)
	{
		/* Wait until STOP Flag is reset */
		/* AutoEnd should be initiate after AF */
		while((pI2C->ISR & I2C_ISR_STOPF) == 0U)
		{
			/* Check for the Timeout */
//			if(timout < TIMEOUT_NACK_STOP) timout++;
//			else return I2C_STATE_TIMEOUT_NACK_STOP;
			
//			if(timout-- == 0) return I2C_STATE_TIMEOUT_NACK_STOP;
			if(IS_TIMEOUT_DONE) return I2C_STATE_TIMEOUT_NACK_STOP;
			
		}

		/* Clear NACKF Flag */
//		pI2C->ICR|=I2C_ICR_NACKCF;
		
		pI2C->ICR|=I2C_ICR_NACKCF|I2C_ICR_STOPCF;
		
		/* Clear STOP Flag */
//		pI2C->ICR|=I2C_ICR_STOPCF;
		
		/* Flush TX register */
		if(pI2C->ISR & I2C_ISR_TXIS)
		{
			pI2C->TXDR = 0U;
		}
		
		if((pI2C->ISR & I2C_ISR_TXE) == 0U)
		{
			pI2C->ISR|=I2C_ISR_TXE;
		}

		/* Clear Configuration Register 2 */
		pI2C->CR2 &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN));

		return I2C_STATE_NACK_STOP;
	}
	else return I2C_STATE_NOT_NACK;
}
//

I2CStateEnum I2C_EEPROM_SetMemAddress(I2C_TypeDef* pI2C, uint32_t i2c_address, uint32_t size_address, uint8_t* p_address);
















#endif

