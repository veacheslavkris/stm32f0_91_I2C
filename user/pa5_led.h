#ifndef PA5_LED_H
#define PA5_LED_H
#include "gpio.h"

#define PORT_LED					GPIOA
#define LED_GREEN_D_POS 	5




#define LED_TOGGLE 	(PORT_LED->ODR ^= 1 << LED_GREEN_D_POS)

#define LED_ON		 	(PORT_LED->BSRR = 1 << LED_GREEN_D_POS)
#define	LED_OFF			(PORT_LED->BRR = 1 << LED_GREEN_D_POS	)



void LedPA5_Init(void);



















#endif



