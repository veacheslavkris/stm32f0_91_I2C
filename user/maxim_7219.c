#include "maxim_7219.h"

#define NUMBERS 			10
#define POSITIONS 		8
#define SYMBOLS_CNT 	40




//uint32_t ary_numbers[NUMBERS]= {DIGIT_0,DIGIT_1,DIGIT_2,DIGIT_3,DIGIT_4,DIGIT_5,DIGIT_6,DIGIT_7,DIGIT_8,DIGIT_9};
uint32_t ary_positions[POSITIONS]= {ADDR_DIG_0,ADDR_DIG_1,ADDR_DIG_2,ADDR_DIG_3,ADDR_DIG_4,ADDR_DIG_5,ADDR_DIG_6,ADDR_DIG_7};

uint32_t ary_symbols[SYMBOLS_CNT];

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
	
	ary_symbols[0]= DIGIT_0;
	ary_symbols[1]= DIGIT_1;
	ary_symbols[2]= DIGIT_2;
	ary_symbols[3]= DIGIT_3;
	ary_symbols[4]= DIGIT_4;
	ary_symbols[5]= DIGIT_5;
	ary_symbols[6]= DIGIT_6;
	ary_symbols[7]= DIGIT_7;
	ary_symbols[8]= DIGIT_8;
	ary_symbols[9]= DIGIT_9;
	ary_symbols[DIGIT_POINT_IX]= DIGIT_POINT;
	ary_symbols[SIGN_A_IX]= SIGN_A;
	ary_symbols[SIGN_E_IX]= SIGN_E;
	ary_symbols[SIGN_C_IX]= SIGN_C;
	ary_symbols[SIGN_F_IX]= SIGN_F;
	ary_symbols[SIGN_H_IX]= SIGN_H;
	ary_symbols[SIGN_L_IX]= SIGN_L;
	ary_symbols[SIGN_P_IX]= SIGN_P;
	ary_symbols[SIGN_U_IX]= SIGN_U;
	ary_symbols[SIGN_UNDSCR_IX]= SIGN_UNDSCR;
	ary_symbols[SIGN_UPRSCR_IX]= SIGN_UPRSCR;
	ary_symbols[SIGN_UND_O_IX]= SIGN_UND_O;
	ary_symbols[SIGN_UPR_O_IX]= SIGN_UPR_O;
	ary_symbols[SIGN_UND_U_IX]= SIGN_UND_U;
	ary_symbols[SIGN_UND_R_IX]= SIGN_UND_R;
	ary_symbols[SIGN_UND_T_IX]= SIGN_UND_T;
	ary_symbols[SIGN_UND_C_IX]= SIGN_UND_C;
	ary_symbols[SIGN_UPR_C_IX]= SIGN_UPR_C;
	ary_symbols[SIGN_UND_N_IX]= SIGN_UND_N;
	ary_symbols[SIGN_UPR_N_IX]= SIGN_UPR_N;
	ary_symbols[SIGN_TIRE_IX]= SIGN_TIRE;
	ary_symbols[SIGN_CLEAR_IX]= REG_DATA_CLEAR;


	
	
}
//

void Max7219_ShowAtPositionNumber(uint32_t position, uint32_t number)
{
	if((position <= POSITIONS) && (number <= NUMBERS))
	{
//		send_bits(((ary_positions[position])<<8)|ary_numbers[number]);
		send_bits(((ary_positions[position])<<8)|ary_symbols[number]);

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

void Max7219_DisplayBcdArray(uint8_t* p_ary_bcd)
{
	uint32_t ix = 7;
//	uint32_t checking_state = 1;
	
	
	for(ix = 7; ix < 8; ix--)
	{
//		if(left_sign == LEFT_ZERO)
//		{
//			// direct copy
////			send_bits(((ary_positions[ix])<<8)|ary_numbers[p_ary_bcd[ix]]);
//			send_bits(((ary_positions[ix])<<8)|ary_symbols[p_ary_bcd[ix]]);

//		}
		
		send_bits(((ary_positions[ix])<<8)|ary_symbols[p_ary_bcd[ix]]);
	}
	
}


