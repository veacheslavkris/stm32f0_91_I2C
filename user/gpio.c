#include "gpio.h"




__INLINE void gpio_init(GPIO_TypeDef* port, uint32_t pin, uint32_t mode, uint32_t otype, uint32_t speed, uint32_t pullupdown, uint32_t alt_func)
{
	// 1. set otype
	port->OTYPER =(port->OTYPER & ~(OTYPER_MASK << pin)) | otype << pin;
	
	// 2. set speed
	port->OSPEEDR = (port->OSPEEDR & (~(OSPEEDR_MASK << (pin*2))))|(speed << (pin*2));

	// 3. set pullup pulldown
	port->PUPDR = (port->PUPDR & (~(PULLUPDOWN_MASK << (pin*2)))) | (pullupdown << (pin*2));
	
	// 4. set alternate function
	if(mode & MODER_ALT_FUNC)
	{
		set_alternate_function_port_pin(port, pin, alt_func);
	}
	
	// 5. set mode
	port->MODER = (port->MODER & ~(MODER_MASK << (pin*2)))|(mode << (pin*2));
}




void GpioSetInterruptMode(uint32_t exti_port, uint32_t pin)
{
	uint32_t ix = pin >> 2;
	uint32_t ix_pin = pin;
	uint32_t pin_mask;
	
	while(ix_pin > 3)
	{
		ix_pin -=4;
	}
	
	SYSCFG->EXTICR[ix] &= ~(EXTI_PORT_MASK << (ix_pin*4));
	SYSCFG->EXTICR[ix] |= exti_port << (ix_pin*4);
	
	pin_mask = 1 << pin;
		
	EXTI->IMR |= pin_mask;
	EXTI->RTSR |= pin_mask;
	EXTI->FTSR |= pin_mask;
}



void set_alternate_function_port_pin(GPIO_TypeDef* port, uint32_t pin, uint32_t alt_func)
{
	uint32_t ix_af = pin >> 3;
	uint32_t rel_ix_pin;
		
	rel_ix_pin = pin;
	
	if(rel_ix_pin > 7)
	{
		rel_ix_pin -= 8;
	}
	
	rel_ix_pin *= 4;
	
	// clear af position  
	port->AFR[ix_af]&= ~(ALT_FUNC_PORT_MASK << rel_ix_pin);
	
	port->AFR[ix_af]|= alt_func << rel_ix_pin;
}

void GpioSetModeOutputStrong(GPIO_TypeDef* port, uint32_t pin, uint32_t speed)
{
	gpio_init(port, pin, MODER_OUTPUT, OTYPER_PUSH_PULL, OSPEEDR_MEDIUM, PULLUPDOWN_CLEAR, ALT_FUNCTION_CLEAR);
}


void GpioSetModeI2C(GPIO_TypeDef* port, uint32_t pin, uint32_t alt_finc_nm)
{
	gpio_init(port, pin, MODER_ALT_FUNC, OTYPER_OPEN_DRAIN, OSPEEDR_CLEAR, PULLUPDOWN_CLEAR, alt_finc_nm);
}


















