#ifndef GPIO_H
#define GPIO_H

#include "stm32f091xc.h"

#define MASK_2_BITS				((uint32_t)0x00000003U)
#define MASK_1_BIT				((uint32_t)0x00000001U)


#define MODER_MASK				MASK_2_BITS
#define PIN_MODE_OUTPUT		((uint32_t)0x00000001U)

#define OTYPER_MASK					MASK_1_BIT
#define OTYPER_PUSH_PULL		((uint32_t)0x00000000U)
#define OTYPER_OPEN_DRAIN		((uint32_t)0x00000001U)



#define OSPEEDR_MASK		MASK_2_BITS
#define PULLUPDOWN_MASK		MASK_2_BITS




#define GPIO_SPEED_LOW		((uint32_t)0x00000000U)
#define GPIO_SPEED_MEDIUM	((uint32_t)0x00000001U)
#define GPIO_SPEED_HIGH		((uint32_t)0x00000003U)

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








//typedef struct
//{
//	GPIO_TypeDef* port;
//	uint32_t pin;
//	uint32_t mode;
//	uint32_t speed;
//	uint32_t
//	

//}GpioPortPin;




__INLINE void set_output_mode_port_pin(GPIO_TypeDef* port, uint32_t pin);
//__INLINE void set_pushpul_mode_port_pin(GPIO_TypeDef* port, uint32_t pin);
__INLINE void set_otyper_port_pin(GPIO_TypeDef* port, uint32_t pin, uint32_t otype);



__INLINE void clear_pullup_pulldown_mode_port_pin(GPIO_TypeDef* port, uint32_t pin);
__INLINE void set_pullup_pulldown_mode_port_pin(GPIO_TypeDef* port, uint32_t pin, uint32_t pullupdown_mode);

__INLINE void set_alternate_function_port_pin(GPIO_TypeDef* port, uint32_t pin, uint32_t alt_func);



void ConfigModeOutputPushPull(GPIO_TypeDef* port, uint32_t pin, uint32_t speed);
void ConfigInterruptMode(uint32_t exti_port, uint32_t pin);















#endif



