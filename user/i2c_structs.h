#ifndef I2C_STRUCTS_H
#define I2C_STRUCTS_H
#include "stm32f091xc.h"

#define I2C_BUFF_ARY_SIZE	16

typedef struct
{
	uint32_t ary_size;
	uint32_t transfer_size;
	uint32_t ix_ary;
	uint8_t ary_data[I2C_BUFF_ARY_SIZE];
	
}I2CStructData;

typedef struct
{
	I2C_TypeDef* pI2C;
	I2CStructData RxBuff;
	I2CStructData TxBuff;
	uint32_t devAddress;

}
I2CStructHandle;


#endif




