#include "gpio.h"




__INLINE void gpio_init(GPIO_TypeDef* port, uint32_t pin, uint32_t mode, uint32_t otype, uint32_t speed, uint32_t pullupdown, uint32_t alt_func)
{
	// 1. set otype
	port->OTYPER =(port->OTYPER & ~(OTYPER_MASK << pin)) | otype << pin;
	
	// 2. set speed
	port->OSPEEDR = (port->OSPEEDR & (~(OSPEEDR_MASK << (pin*2))))|(speed << (pin*2));

	// 3. set pullup pulldown
	port->PUPDR = (port->PUPDR & (~(PUPDR_MASK << (pin*2)))) | (pullupdown << (pin*2));
	
	// 4. set alternate function
	if(mode & MODER_ALT_FUNC)
	{
		set_alternate_function_port_pin(port, pin, alt_func);
	}
	
	// 5. set mode
	port->MODER = (port->MODER & ~(MODER_MASK << (pin*2)))|(mode << (pin*2));
}




void GpioSetInputModeInterrupt(GPIO_TypeDef* port, uint32_t exti_port, uint32_t pin, uint32_t edge_itr)
{
	uint32_t ix = pin >> 2;
	uint32_t ix_pin = pin;
	uint32_t pin_mask;
	
	gpio_init(port, pin, MODER_INPUT, OTYPER_PUSH_PULL, OSPEEDR_LOW, PUPDR_CLEAR, ALT_FUNCTION_CLEAR);
	
	while(ix_pin > 3)
	{
		ix_pin -=4;
	}
	
	SYSCFG->EXTICR[ix] &= ~(EXTI_PORT_MASK << (ix_pin*4));
	SYSCFG->EXTICR[ix] |= exti_port << (ix_pin*4);
	
	pin_mask = 1 << pin;
		
	EXTI->IMR |= pin_mask;
	
	if(edge_itr & EDGE_RISING_INTERRUPT) EXTI->RTSR |= pin_mask;
	if(edge_itr & EDGE_FALLING_INTERRUPT) EXTI->FTSR |= pin_mask;
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
	gpio_init(port, pin, MODER_OUTPUT, OTYPER_PUSH_PULL, OSPEEDR_MEDIUM, PUPDR_CLEAR, ALT_FUNCTION_CLEAR);
}


void GpioSetModeI2C(GPIO_TypeDef* port, uint32_t pinScl, uint32_t pinSda, uint32_t alt_finc_nm)
{
	gpio_init(port, pinScl, MODER_ALT_FUNC, OTYPER_OPEN_DRAIN, OSPEEDR_CLEAR, PUPDR_CLEAR, alt_finc_nm);
	gpio_init(port, pinSda, MODER_ALT_FUNC, OTYPER_OPEN_DRAIN, OSPEEDR_CLEAR, PUPDR_CLEAR, alt_finc_nm);

}


void GpioSetModeUart(GPIO_TypeDef* port, uint32_t pinTx, uint32_t pinRx, uint32_t alt_finc_nm)
{
	gpio_init(port, pinTx, MODER_ALT_FUNC, OTYPER_PUSH_PULL, OSPEEDR_HIGH, PUPDR_PULLUP, alt_finc_nm);
	gpio_init(port, pinRx, MODER_ALT_FUNC, OTYPER_PUSH_PULL, OSPEEDR_HIGH, PUPDR_PULLUP, alt_finc_nm);

}















