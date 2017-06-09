#ifndef A_I2C_H
	#define A_I2C_H

#include "stm32f091xc.h"
#include "i2c_structs.h"	
	
	
#define  I2C_GENERATE_STOP              I2C_CR2_STOP
#define  I2C_GENERATE_START_READ        (uint32_t)(I2C_CR2_START | I2C_CR2_RD_WRN)
#define  I2C_GENERATE_START_WRITE       I2C_CR2_START

#define  I2C_RELOAD_MODE                I2C_CR2_RELOAD
#define  I2C_AUTOEND_MODE               I2C_CR2_AUTOEND
	
	
/* I2C slave address to communicate with */
#define TMP275_ADDRESS	(uint32_t)0x48

#define TRANSFER_DIR_WRITE	(uint32_t)0
#define TRANSFER_DIR_READ		(uint32_t)1

#define REG_TEMP_PTR     (uint32_t)0x00u
#define REG_CONF_PTR     (uint32_t)0x01u


void I2C_ConfigMstrTimingPe(I2C_TypeDef* pI2C);

uint32_t I2C_MasterStartSendOneByteAutoEnd(I2C_TypeDef* pI2C, uint32_t slaveAddress, uint32_t data_byte);
	
float I2C_MasterStartGetTempAutoEnd(I2C_TypeDef* pI2C, uint32_t slaveAddress);
























#endif

