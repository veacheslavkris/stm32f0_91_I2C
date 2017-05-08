#include "stm32f0xx.h"
#include "sys_clock.h"
#include "gpio.h"

//#include "gpio_led.h"
//#include "gpio_exti.h"
#include "I2C.h"
#include "maxim_7219.h"


/* Private define ------------------------------------------------------------*/
#define LED_GREEN_A5_D_POS 		5
#define LED_GREEN_A5_B_POS		GPIO_ODR_5
#define BTN_C13_PIN_POS 			13

#define PIN_CLK_PC0_D_POS			((uint32_t)0)
#define PIN_DOUT_PC1_D_POS		((uint32_t)1)
#define PIN_LATCH_PC3_D_POS		((uint32_t)3)


void delay_systick(uint32_t ms);
	

void init_led_gpio(void);
void ConfigureExternalIT(void);

void init_max7219_gpio(void);

void LatchMax7219Off(void);

void LatchMax7219On(void);

void ClkMax7219Off(void);

void ClkMax7219On(void);

void SetDataPin(uint32_t val);

void start_max7219(void);
