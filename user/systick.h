#ifndef SYSTICK_H
#define SYSTICK_H
#include "stm32f091xc.h"

void SysTick_Handler(void);
uint32_t GetSystickCount(void);

uint32_t GetTicksSince(uint32_t start);

void DelaySystick(uint32_t ms);











#endif



