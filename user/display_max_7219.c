#include "display_max_7219.h"



void DisplayMax7219_TT_tt(uint32_t displ_num, StructFloatToBcd* structFloatToBcd)
{
	// 1. Prepare simbols array
	
	if(displ_num == 0)
	{
		structFloatToBcd->ary_bcd[7] = SIGN_CLEAR_IX;
		structFloatToBcd->ary_bcd[6] = SIGN_CLEAR_IX;
		structFloatToBcd->ary_bcd[5] = SIGN_CLEAR_IX;
		structFloatToBcd->ary_bcd[4] = SIGN_CLEAR_IX;
		
		Max7219_DisplayBcdArray(structFloatToBcd->ary_bcd);

	}
	else if(displ_num == 1)
	{
	
	
	}
	
	
	
	
//void Max7219_DisplayBcdArray(EnumLeftSign left_sign, uint8_t* p_ary_bcd)


}





























