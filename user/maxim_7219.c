#include "maxim_7219.h"

#define NUMBERS 	10
#define POSITIONS 8




uint32_t ary_numbers[NUMBERS]= {DIGIT_0,DIGIT_1,DIGIT_2,DIGIT_3,DIGIT_4,DIGIT_5,DIGIT_6,DIGIT_7,DIGIT_8,DIGIT_9};
uint32_t ary_positions[POSITIONS]= {ADDR_DIG_0,ADDR_DIG_1,ADDR_DIG_2,ADDR_DIG_3,ADDR_DIG_4,ADDR_DIG_5,ADDR_DIG_6,ADDR_DIG_7};


/******************************************************************************/
/*                          INITIALIZATION PARAMS                             */
/******************************************************************************/

#define PORT_CLK			GPIOC 
#define PIN_CLK_POS 	0

#define PORT_DOUT			GPIOC
#define PIN_DOUT_POS	1

#define PORT_LATCH		GPIOB 
#define PIN_LATCH_POS	0


#define  LATCH_OFF				(PORT_LATCH->BRR = 1 << PIN_LATCH_POS	)
#define  LATCH_ON					(PORT_LATCH->BSRR = 1 << PIN_LATCH_POS)
#define  CLKMAX_OFF				(PORT_CLK->BRR = 1 << PIN_CLK_POS)
#define  CLKMAX_ON				(PORT_CLK->BSRR = 1 << PIN_CLK_POS)
#define  SET_DATA_PIN_1		(PORT_DOUT->BSRR = 1 << PIN_DOUT_POS)	
#define  SET_DATA_PIN_0		(PORT_DOUT->BRR = 1 << PIN_DOUT_POS)

/******************************************************************************/
/*                                                                            */
/******************************************************************************/

void Max7219_Init(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	
	GpioSetModeOutputStrong(PORT_CLK, PIN_CLK_POS, OSPEEDR_HIGH);
	GpioSetModeOutputStrong(PORT_DOUT, PIN_DOUT_POS, OSPEEDR_HIGH);
	GpioSetModeOutputStrong(PORT_LATCH, PIN_LATCH_POS, OSPEEDR_HIGH);
	
	
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

__INLINE void latch_max7219(void)
{
			LATCH_ON;
			LATCH_OFF;
}
//

__INLINE void config_max7219(uint8_t reg_addr, uint8_t reg_data)
{
    send_bits((reg_addr<<8)|reg_data);
}
//

__INLINE void send_bits(uint32_t val)
{
	  uint32_t test_bit = 0x0800; // bit 11 position set
    uint32_t res = 0;
    
		LATCH_OFF;
   
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

__INLINE void set_data_bit(uint32_t val)
{
	CLKMAX_OFF;
		
	if(val) SET_DATA_PIN_1;
	else SET_DATA_PIN_0;
	
	CLKMAX_ON;

	CLKMAX_OFF;
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



