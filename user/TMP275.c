#include "TMP275.h"

#define TMP275_ADDRESS	(uint32_t)0x48

#define RX_BUFF_ARY_SIZE	2
#define TX_BUFF_ARY_SIZE	2
	
I2CStructHandle handleTMP275;
TMP275ExecutionResult execResult;

uint8_t ary_rx_buff[RX_BUFF_ARY_SIZE]={0,0};
uint8_t ary_tx_buff[TX_BUFF_ARY_SIZE]={0,0}; 


void init_handle_tpm275(void)
{
	handleTMP275.pI2C = I2C;
	handleTMP275.devAddress = TMP275_ADDRESS;
	
	handleTMP275.RxBuff.ary_size = RX_BUFF_ARY_SIZE;
	handleTMP275.RxBuff.p_ary_data = ary_rx_buff;
	
	
	handleTMP275.TxBuff.ary_size = TX_BUFF_ARY_SIZE;
	handleTMP275.TxBuff.p_ary_data = ary_tx_buff;
	
	clear_buffer(&handleTMP275.RxBuff);
	clear_buffer(&handleTMP275.TxBuff);
}

void TMP275_Init(void)
{
	i2c_init();
	init_handle_tpm275();
}

TMP275ExecutionResult* TMP275GetTemperature(void)
{
	uint8_t temp_1 = 0x00;
	uint8_t temp_2 = 0x00;
	uint32_t temp_16 = 0;
	
	I2CStateEnum i2c_state;

	I2C_ClearHandleBuffers(&handleTMP275);
	handleTMP275.RxBuff.transfer_size=2;

	i2c_state = HAL_I2C_Master_Receive(&handleTMP275);

	if(i2c_state==I2C_STATE_TRANSFER_DONE)
	{

		temp_1 = handleTMP275.RxBuff.p_ary_data[0];
		temp_2 = handleTMP275.RxBuff.p_ary_data[1];

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



