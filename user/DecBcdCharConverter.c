#include "DecBcdCharConverter.h"



void ConvertDecToBcd(StructDecToBcd*  structDecToBcd)
{
	uint32_t ix;
	
	// zeroed array 
	for (ix = 0; ix < ARY_BCD_SIZE; ix++)
	{
		structDecToBcd->ary_bcd[ix] = 0;
	}


convert_dec_to_bcd(structDecToBcd->dec, structDecToBcd->ary_bcd);
}

void convert_dec_to_bcd(uint32_t dec, uint8_t* ary_bcd)
{
	uint32_t ix;
	uint32_t cur_power;
	uint32_t tmp_power;

	// ix is also dec_power 
	for (ix = ARY_BCD_SIZE - 1; ix > 0; ix--)
	{
		tmp_power = ix;
		cur_power = 1;

		// form current investigated power of dec
		while (tmp_power)
		{
			cur_power *= 10;

			tmp_power--;
		}

		tmp_power = 0;

		// count of current power
		while (dec >= cur_power)
		{
			tmp_power++;
			dec -= cur_power;
		}

		ary_bcd[ix] = tmp_power;
	}

	ary_bcd[ix] = dec;
}

void ConvertFloatToBcd(StructFloatToBcd * structFloatToBcd)
{
	//float value = 37.777779;
	//float rounded = ((int)(value * 100 + .5) / 100.0);

	uint32_t float_multipl=1;
	uint32_t ix = structFloatToBcd->float_count;
	uint32_t dec_float;

	while (ix > 0)
	{
		float_multipl *= 10;
		ix--;
	}

	dec_float = (int)(structFloatToBcd->flt * float_multipl);

	convert_dec_to_bcd(dec_float, structFloatToBcd->ary_bcd);




}




