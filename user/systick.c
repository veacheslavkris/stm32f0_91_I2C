#include "systick.h"

volatile uint32_t systick_count = 0;
#define SYSTICK_MAX	0x00FFFFFFU



void SysTick_Handler(void)
{
	systick_count++;
}


uint32_t GetSystickCount(void)
{
	return systick_count;
}	

uint32_t GetTicksSince(uint32_t start)
{
	if(start <= systick_count)
	{		
		return(systick_count - start);
	}
	else
	{		
		return ((SYSTICK_MAX-start) + systick_count);
	}
}

void DelaySystick(uint32_t ms)
{
	uint32_t start = systick_count;
	
	while(GetTicksSince(start) <= ms) continue;
}










