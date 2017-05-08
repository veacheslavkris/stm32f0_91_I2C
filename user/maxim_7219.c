#include "maxim_7219.h"


extern void LatchMax7219Off(void);
extern void LatchMax7219On(void);
extern void ClkMax7219Off(void);
extern void ClkMax7219On(void);
extern void SetDataPin(uint32_t);

void Max7219_Init(void)
{
    config_max7219(ADDR_DECODE_MODE, NO_DECODE_FOR_DIGITS);
    config_max7219(ADDR_INTENSITY, INTENSITY_MAX);
    config_max7219(ADDR_SCAN_LIMIT, DISPLAY_DIGIT_01234567 );
    config_max7219(ADDR_SHUTDOWN, SD_NORMAL_OPERATION);
    config_max7219(ADDR_DISPLAY_TEST, DT_NORMAL_OPERATION);

}



void latch_max7219(void)
{
    LatchMax7219On();
    LatchMax7219Off();
    
    
}

void config_max7219(uint8_t reg_addr, uint8_t reg_data)
{
    send_bits((reg_addr<<8)|reg_data);
}

void Max7219_SetDigitSegment(uint8_t reg_digit, uint8_t reg_data_seg)
{
    send_bits((reg_digit<<8)|reg_data_seg);
}




void send_bits(uint32_t val)
{
	  uint32_t test_bit = 0x0800; // bit 11 position set
    uint32_t res = 0;
    
		LatchMax7219Off();
    

    
    while(test_bit)
    {
        res = val & test_bit;
        
        if(res == 0)
        {
            set_data_bit(0);
        }
        else
        {
            set_data_bit(1);
        }
        
        test_bit = test_bit >> 1;
    
    
    }
    
    latch_max7219();

}

void set_data_bit(uint32_t val)
{
    ClkMax7219Off();
    
    SetDataPin(val);
    
    ClkMax7219On();
    ClkMax7219Off();

}

void Max7219_ClearAllDigits(void)
{
    Max7219_SetDigitSegment(ADDR_DIG_0, REG_DATA_CLEAR);
    Max7219_SetDigitSegment(ADDR_DIG_1, REG_DATA_CLEAR);
    Max7219_SetDigitSegment(ADDR_DIG_2, REG_DATA_CLEAR);
    Max7219_SetDigitSegment(ADDR_DIG_3, REG_DATA_CLEAR);
    Max7219_SetDigitSegment(ADDR_DIG_4, REG_DATA_CLEAR);
    Max7219_SetDigitSegment(ADDR_DIG_5, REG_DATA_CLEAR);
    Max7219_SetDigitSegment(ADDR_DIG_6, REG_DATA_CLEAR);
    Max7219_SetDigitSegment(ADDR_DIG_7, REG_DATA_CLEAR);

}



