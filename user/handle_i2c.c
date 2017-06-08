#include "handle_i2c.h"

/*********************************************************************************/
/*                                   DEFINITIONS                                 */
/*********************************************************************************/

#define PI2C	I2C2

I2CStructHandle handleI2C2;


void I2C2_InitHandle(void)
{
	handleI2C2.pI2C = PI2C;
	handleI2C2.RxBuff.ary_size = I2C_BUFF_ARY_SIZE;
	handleI2C2.TxBuff.ary_size = I2C_BUFF_ARY_SIZE;

}



























