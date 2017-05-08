#include "stm32f0xx.h"
#include "sys_clock.h"
#include "I2C.h"
#include "maxim_7219.h"
#include "rtc.h"
#include "hardware.h"
#include "uart.h"

/* Private define ------------------------------------------------------------*/



void delay_systick(uint32_t ms);
	

void LatchMax7219Off(void);
void LatchMax7219On(void);
void ClkMax7219Off(void);
void ClkMax7219On(void);
void SetDataPin(uint32_t val);
void start_max7219(void);

void cycle_digits(void);
