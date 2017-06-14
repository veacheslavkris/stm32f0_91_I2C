#include "TMP275.h"

#define TMP275_ADDRESS	(uint32_t)0x48
	
	
I2CStructHandle handleI2C;
TMP275ExecutionResult execResult;

void clear_buffer(I2CStructData* pI2CStructData)
{
	uint32_t ix = 0;
	
	for(ix=0; ix< pI2CStructData->ary_size; ix++)
	{
		pI2CStructData->ary_data[ix]=0;
	}
	
	pI2CStructData->ix_ary = 0;
	pI2CStructData->transfer_size=0;
}

void init_handle(void)
{
	handleI2C.pI2C = I2C;
	handleI2C.devAddress = TMP275_ADDRESS;
	
	handleI2C.RxBuff.ary_size = I2C_BUFF_ARY_SIZE;
	handleI2C.TxBuff.ary_size = I2C_BUFF_ARY_SIZE;
	
	clear_buffer(&handleI2C.RxBuff);
	clear_buffer(&handleI2C.TxBuff);
}

void TMP275_Init(void)
{
	i2c_init();
	init_handle();
}

I2CStructHandle* TMP275_GetHandle(void)
{
	return &handleI2C;
}


TMP275ExecutionResult* TMP275GetTemperature(void)
{
	uint8_t temp_1 = 0x00;
	uint8_t temp_2 = 0x00;
	uint32_t temp_16 = 0;
	
	I2CStateEnum i2c_state;

	clear_buffer(&handleI2C.RxBuff);
	clear_buffer(&handleI2C.TxBuff);
	handleI2C.RxBuff.transfer_size=2;

	i2c_state = HAL_I2C_Master_Receive(&handleI2C);

	if(i2c_state==I2C_STATE_TRANSFER_DONE)
	{

		temp_1 = handleI2C.RxBuff.ary_data[0];
		temp_2 = handleI2C.RxBuff.ary_data[1];

		temp_16 = (temp_1<<4)|(temp_2>>4);

		execResult.temprt = temp_16/16.0f;
		execResult.execState = MEASUREMENT_SUCCESSFULL;

	}
	else
	{
		execResult.temprt = 0;
		execResult.execState = MEASUREMENT_FAIL;
	}		
	
	return &execResult;

}



