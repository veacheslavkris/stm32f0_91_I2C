#ifndef DEC_BCD_CHAR_CONVERTER_H
#define DEC_BCD_CHAR_CONVERTER_H

#include "stm32f091xc.h"


#define ARY_BCD_SIZE	10


typedef struct
{
	uint32_t dec;
	uint8_t ary_bcd[ARY_BCD_SIZE];

}StructDecToBcd;

typedef struct
{
	float flt;
	uint32_t float_count;
	uint8_t ary_bcd[ARY_BCD_SIZE];
	
}StructFloatToBcd;



void ConvertDecToBcd(StructDecToBcd*  structDecToBcd);

void convert_dec_to_bcd(uint32_t dec, uint8_t* ary_bcd);


void ConvertFloatToBcd(StructFloatToBcd*  structDecToBcd);











#endif




