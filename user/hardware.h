#ifndef HARDWARE_H
#define HARDWARE_H

#include "stm32f091xc.h"
#include "gpio.h"


//#define LED_GREEN_A5_D_POS 		5
//#define LED_GREEN_A5_B_POS		GPIO_ODR_5



void HWInitGpio(void);

void init_rcc(void);


//void init_led_gpio(void);
void init_I2C_gpio(void);





















#endif



