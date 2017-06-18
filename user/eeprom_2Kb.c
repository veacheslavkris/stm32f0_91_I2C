#include "eeprom_2Kb.h"

#define EEPROM_ADDRESS	(uint32_t)0x50

#define BUFF_ARY_SIZE	256

I2CStructHandle handleI2C_EEPROM;

uint8_t eeprom_ary_rx_buff[BUFF_ARY_SIZE];
uint8_t eeprom_ary_tx_buff[BUFF_ARY_SIZE];

const uint32_t cnt_bytes_mem_address = 2;
uint8_t ary_mem_address[cnt_bytes_mem_address];

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

I2CStateEnum EEPROM_Read(uint32_t start_pos, uint32_t count_for_receive)
{
	I2CStateEnum state;
	
	// start position for reading
	ary_mem_address[1] = (uint8_t)start_pos;
	ary_mem_address[0] = (uint8_t)(start_pos >> 8);

	handleI2C_EEPROM.RxBuff.transfer_size = count_for_receive;

	state =  I2C_EEPROM_SetMemAddress(handleI2C_EEPROM.pI2C, handleI2C_EEPROM.devAddress, cnt_bytes_mem_address, ary_mem_address);
	
	if(state == I2C_STATE_MEM_ADR_TRANSFER_DONE)
	{
		state = HAL_I2C_Master_Receive(&handleI2C_EEPROM);
	}
	
	return state;
}

I2CStateEnum EEPROM_Write(uint32_t start_pos, uint32_t count_for_write)
{
	I2CStateEnum state;
	
	// start position for writing
	ary_mem_address[1] = (uint8_t)start_pos;
	ary_mem_address[0] = (uint8_t)(start_pos >> 8);

	handleI2C_EEPROM.TxBuff.transfer_size = count_for_write;

	state =  I2C_EEPROM_SetMemAddress_2(handleI2C_EEPROM.pI2C, handleI2C_EEPROM.devAddress, cnt_bytes_mem_address, ary_mem_address);
	
	if(state == I2C_STATE_MEM_ADR_TRANSFER_DONE)
	{
		state = HAL_I2C_Master_Transmit(&handleI2C_EEPROM);
	}
	
	return state;
}

uint8_t* EEPROM_GetRxBufferArray()
{
	return eeprom_ary_rx_buff;
}

uint8_t* EEPROM_GetTxBufferArray()
{
	return eeprom_ary_tx_buff;
}

void EEPROM_ClearRxTxBuffers(void)
{
		I2C_ClearHandleBuffers(&handleI2C_EEPROM);
}

I2CStructHandle* EEPROM_GetHandle(void)
{
	return &handleI2C_EEPROM;
}
