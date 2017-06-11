#include "handle_i2c.h"

/*********************************************************************************/
/*                                   DEFINITIONS                                 */
/*********************************************************************************/

/* I2C slave address to communicate with */
#define TMP275_ADDRESS	(uint32_t)0x48
#define PI2C	I2C2



I2CStructHandle handleI2C2;


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

void I2C2_InitHandle(void)
{
	handleI2C2.pI2C = PI2C;
	handleI2C2.devAddress = TMP275_ADDRESS;
	
	handleI2C2.RxBuff.ary_size = I2C_BUFF_ARY_SIZE;
	handleI2C2.TxBuff.ary_size = I2C_BUFF_ARY_SIZE;
	
	clear_buffer(&handleI2C2.RxBuff);
	clear_buffer(&handleI2C2.TxBuff);

}

I2CStructHandle* I2C2_GetHandle(void)
{
	return &handleI2C2;
}

void I2C2_ClearHandle(void)
{
	clear_buffer(&handleI2C2.RxBuff);
	clear_buffer(&handleI2C2.TxBuff);
}

























