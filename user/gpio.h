#ifndef GPIO_H
#define GPIO_H

#include "stm32f091xc.h"

#define MASK_2_BITS				((uint32_t)0x00000003U)
#define MASK_1_BIT				((uint32_t)0x00000001U)


#define MODER_MASK				MASK_2_BITS
#define MODER_INPUT			((uint32_t)0x00000000U)
#define MODER_OUTPUT		((uint32_t)0x00000001U)
#define MODER_ALT_FUNC	((uint32_t)0x00000002U)


#define OTYPER_MASK					MASK_1_BIT
#define OTYPER_PUSH_PULL		((uint32_t)0x00000000U)
#define OTYPER_OPEN_DRAIN		((uint32_t)0x00000001U)



#define OSPEEDR_MASK		MASK_2_BITS
#define OSPEEDR_LOW			((uint32_t)0x00000000U)
#define OSPEEDR_MEDIUM	((uint32_t)0x00000001U)
#define OSPEEDR_HIGH		((uint32_t)0x00000003U)

#define OSPEEDR_CLEAR		((uint32_t)0x00000000U)


#define PUPDR_MASK		MASK_2_BITS
#define PUPDR_CLEAR	((uint32_t)0x00000000U)
#define PUPDR_PULLUP			((uint32_t)0x00000001U)
#define PUPDR_PULLDOWN		((uint32_t)0x00000002U)




#define ALT_FUNCTION_CLEAR	((uint32_t)0x00000000U)

#define EDGE_RISING_INTERRUPT		((uint32_t)0x00000001U)
#define EDGE_FALLING_INTERRUPT	((uint32_t)0x00000002U)
#define EDGE_RISING_FALLING_ITR	(EDGE_RISING_INTERRUPT|EDGE_FALLING_INTERRUPT)



#define EXTI_PA	((uint32_t)0x00000000U)
#define EXTI_PB	((uint32_t)0x00000001U)
#define EXTI_PC	((uint32_t)0x00000002U)
#define EXTI_PD	((uint32_t)0x00000003U)
#define EXTI_PE	((uint32_t)0x00000004U)
#define EXTI_PF	((uint32_t)0x00000005U)

#define EXTI_PORT_MASK		((uint32_t)0x0000000FU)

#define ALT_FUNC_PORT_MASK	((uint32_t)0x0000000FU)

#define ALT_FUNC_0	((uint32_t)0x00000000U)
#define ALT_FUNC_1	((uint32_t)0x00000001U)
#define ALT_FUNC_2	((uint32_t)0x00000002U)
#define ALT_FUNC_3	((uint32_t)0x00000003U)
#define ALT_FUNC_4	((uint32_t)0x00000004U)
#define ALT_FUNC_5	((uint32_t)0x00000005U)
#define ALT_FUNC_6	((uint32_t)0x00000006U)
#define ALT_FUNC_7	((uint32_t)0x00000007U)


void set_alternate_function_port_pin(GPIO_TypeDef* port, uint32_t pin, uint32_t alt_func);
void gpio_init(GPIO_TypeDef* port, uint32_t pin, uint32_t mode, uint32_t otype, uint32_t speed, uint32_t pullupdown_mode, uint32_t alt_func);

void GpioSetModeOutputStrong(GPIO_TypeDef* port, uint32_t pin, uint32_t speed);
void GpioSetModeI2C(GPIO_TypeDef* port, uint32_t pin, uint32_t alt_finc_nm);
void GpioSetModeUart(GPIO_TypeDef* port, uint32_t pinTx, uint32_t pinRx, uint32_t alt_finc_nm);



void GpioSetInputModeInterrupt(GPIO_TypeDef* port, uint32_t exti_port, uint32_t pin, uint32_t edge_itr);










#endif



