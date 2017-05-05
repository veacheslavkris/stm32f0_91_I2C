#ifndef I2C_H
	#define I2C_H

//	#include "stm32f0xx.h"
	#include "stm32f091xc.h"
	
	#include "stdint.h"
	
/* I2C slave address to communicate with */
#define TMP275_ADDRESS	(uint32_t)0x48

#define TRANSFER_DIR_WRITE	(uint32_t)0
#define TRANSFER_DIR_READ		(uint32_t)1

#define REG_TEMP_PTR     (uint32_t)0x00u
#define REG_CONF_PTR     (uint32_t)0x01u


void I2C2GPIOConfigure(void);

void I2C2MasterConfigure(I2C_TypeDef* pI2C);

uint32_t I2C2MasterSendStartOneByteAutoEnd(I2C_TypeDef* pI2C, uint32_t slaveAddress, uint32_t data_byte);
	
float I2C2MasterSendStartGetTempAutoEnd(I2C_TypeDef* pI2C, uint32_t slaveAddress);
























#endif

