#include "stm32f0xx.h"
#include "sys_clock.h"
#include "I2C.h"
#include "maxim_7219.h"
#include "rtc.h"
#include "hardware.h"
#include "uart8.h"
#include "pc13btn.h"
#include "pa5_led.h"



/* Private define ------------------------------------------------------------*/



void delay_systick(uint32_t ms);
	





void cycle_digits(void);
