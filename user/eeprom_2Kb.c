#include "eeprom_2Kb.h"

#define EEPROM_ADDRESS	(uint32_t)0x50

#define BUFF_ARY_SIZE	256

I2CStructHandle handleI2C_EEPROM;


uint8_t eeprom_ary_rx_buff[BUFF_ARY_SIZE];
uint8_t eeprom_ary_tx_buff[BUFF_ARY_SIZE];


void init_handle_eeprom(void)
{
	handleI2C_EEPROM.pI2C = I2C;
	handleI2C_EEPROM.devAddress = EEPROM_ADDRESS;
	
	handleI2C_EEPROM.RxBuff.ary_size = BUFF_ARY_SIZE;
	handleI2C_EEPROM.RxBuff.p_ary_data = eeprom_ary_rx_buff;
	
	handleI2C_EEPROM.TxBuff.ary_size = BUFF_ARY_SIZE;
	handleI2C_EEPROM.TxBuff.p_ary_data = eeprom_ary_tx_buff;

	I2C_ClearHandleBuffers(&handleI2C_EEPROM);
}

void EEPROM_2Kb_Init(void)
{
	i2c_init();
	init_handle_eeprom();

}

I2CStructHandle* EEPROM_2Kb_Read(uint32_t start_pos, uint32_t count)
{
	I2CStateEnum state;
	
	// start position for reading
	handleI2C_EEPROM.TxBuff.p_ary_data[0] = start_pos;
	handleI2C_EEPROM.RxBuff.transfer_size = count;

//	state = I2C_EEPROM_SetMemAddress(&handleI2C_EEPROM);
	
//	if(state == I2C_STATE_TRANSFER_DONE)
//	{
//		state = HAL_I2C_Master_Receive(&handleI2C_EEPROM);
//	}

	state = HAL_I2C_Master_Receive(&handleI2C_EEPROM);
	
	return &handleI2C_EEPROM;
	

}

