#include "maxim_7219.h"

#define NUMBERS 	10
#define POSITIONS 8


uint32_t ary_numbers[NUMBERS]= {DIGIT_0,DIGIT_1,DIGIT_2,DIGIT_3,DIGIT_4,DIGIT_5,DIGIT_6,DIGIT_7,DIGIT_8,DIGIT_9};
uint32_t ary_positions[POSITIONS]= {ADDR_DIG_0,ADDR_DIG_1,ADDR_DIG_2,ADDR_DIG_3,ADDR_DIG_4,ADDR_DIG_5,ADDR_DIG_6,ADDR_DIG_7};


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
//

void Max7219_ShowAtPositionNumber(uint32_t position, uint32_t number)
{
	if((position <= POSITIONS) && (number <= NUMBERS))
	{
		send_bits(((ary_positions[position])<<8)|ary_numbers[number]);
	}
	else
	{
		show_err_on_display_0();
		show_err_on_display_1();
	}
}
//

void show_err_on_display_0()
{
	send_bits(((ary_positions[3])<<8)|SIGN_E);
	send_bits(((ary_positions[2])<<8)|SIGN_UND_R);
	send_bits(((ary_positions[1])<<8)|SIGN_UND_R);
	send_bits(((ary_positions[0])<<8)|SIGN_UND_O);
}
//

void show_err_on_display_1()
{
	send_bits(((ary_positions[7])<<8)|SIGN_E);
	send_bits(((ary_positions[6])<<8)|SIGN_UND_R);
	send_bits(((ary_positions[5])<<8)|SIGN_UND_R);
	send_bits(((ary_positions[4])<<8)|SIGN_UND_O);
}
//

void latch_max7219(void)
{
    LatchMax7219On();
    LatchMax7219Off();
}
//

void config_max7219(uint8_t reg_addr, uint8_t reg_data)
{
    send_bits((reg_addr<<8)|reg_data);
}
//

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
//

void set_data_bit(uint32_t val)
{
    ClkMax7219Off();
    
    SetDataPin(val);
    
    ClkMax7219On();
    ClkMax7219Off();

}
//

void Max7219_ClearAllDigits(void)
{
	send_bits((ADDR_DIG_0<<8)|REG_DATA_CLEAR);
	send_bits((ADDR_DIG_1<<8)|REG_DATA_CLEAR);
	send_bits((ADDR_DIG_2<<8)|REG_DATA_CLEAR);
	send_bits((ADDR_DIG_3<<8)|REG_DATA_CLEAR);
	send_bits((ADDR_DIG_4<<8)|REG_DATA_CLEAR);
	send_bits((ADDR_DIG_5<<8)|REG_DATA_CLEAR);
	send_bits((ADDR_DIG_6<<8)|REG_DATA_CLEAR);
	send_bits((ADDR_DIG_7<<8)|REG_DATA_CLEAR);
}
//



