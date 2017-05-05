#include "gpio.h"





__INLINE void set_output_mode_port_pin(GPIO_TypeDef* port, uint32_t pin)
{
	// clear pin's mode 
	// set pin's mode
	port->MODER = (port->MODER & ~(MODER_MASK << (pin*2)))|(PIN_MODE_OUTPUT << (pin*2));
}


//__INLINE void set_pushpul_mode_port_pin(GPIO_TypeDef* port, uint32_t pin)
//{
//	port->OTYPER &= ~(OTYPER_MASK << pin);
//}

__INLINE void set_otyper_port_pin(GPIO_TypeDef* port, uint32_t pin, uint32_t otype)
{
	port->OTYPER &= ~(OTYPER_MASK << pin);
	port->OTYPER |= otype << pin;

}



__INLINE void clear_pullup_pulldown_mode_port_pin(GPIO_TypeDef* port, uint32_t pin)
{
	port->PUPDR = port->PUPDR & ~(PULLUPDOWN_MASK << (pin*2));
}


__INLINE void set_pullup_pulldown_mode_port_pin(GPIO_TypeDef* port, uint32_t pin, uint32_t pullupdown_mode)
{
	clear_pullup_pulldown_mode_port_pin(port, pin);
	
	port->PUPDR|= pullupdown_mode << (pin *2);

}









void ConfigModeOutputPushPull(GPIO_TypeDef* port, uint32_t pin, uint32_t speed_type)
{
	// set output mode
	set_output_mode_port_pin(port, pin);
	
	// set speed:
	// - clear pin's speed 
	// - set pin's speed
	port->OSPEEDR = (port->OSPEEDR & (~(OSPEEDR_MASK << (pin*2))))|(speed_type << (pin*2));
	
	// set pushpull mode
//	set_pushpul_mode_port_pin(port, pin);
	set_otyper_port_pin(port, pin, OTYPER_PUSH_PULL);
	
	
	// set no_pullup no_puldown
	clear_pullup_pulldown_mode_port_pin(port, pin);
	
}
	


void ConfigInterruptMode(uint32_t exti_port, uint32_t pin)
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



__INLINE void set_alternate_function_port_pin(GPIO_TypeDef* port, uint32_t pin, uint32_t alt_func)
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

























