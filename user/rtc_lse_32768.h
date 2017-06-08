
#ifndef RTC_LSE_32768_H
#define RTC_LSE_32768_H
#include "stdint.h"

typedef struct
{
  uint8_t Hours;           
  uint8_t Minutes;          
  uint8_t Seconds;          
  uint32_t DayLightSaving;  

}RTC_TimeTypeDef; 
  
typedef struct
{
  uint8_t WeekDay;  
  uint8_t Month;
  uint8_t Date; 
  uint8_t Year; 
	
}RTC_DateTypeDef;

typedef struct
{
  uint8_t HoursT;           
  uint8_t HoursU;
	uint8_t MinutesT;          
  uint8_t MinutesU;
	uint8_t SecondsT;          
  uint8_t SecondsU;
	uint32_t DayLightSaving;  

}RTC_BCD_TimeTypeDef; 
  
typedef struct
{
  uint8_t WeekDayU;  
  uint8_t MonthT;
  uint8_t MonthU;
	uint8_t DateT; 
  uint8_t DateU;
	uint8_t YearT; 
	uint8_t YearU;
	
}RTC_BCD_DateTypeDef;

void RtcInitLse(void);
void RtcSetDateTime(uint8_t*);
void RtcGetCurDateTime_BIN(RTC_TimeTypeDef*, RTC_DateTypeDef*);
void RtcGetCurDateTime_BCD(RTC_BCD_TimeTypeDef*, RTC_BCD_DateTypeDef*);

void RtcLse32768_Init(void);

__STATIC_INLINE void enable_RTC_write_access(void)
{
	/* Enable PWR clock */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	
	/* Enable write access to Backup domain */
	PWR->CR |= PWR_CR_DBP; /* (4) */
	
	/* Wait for Backup domain Write protection disable */
	while((PWR->CR & PWR_CR_DBP)!=PWR_CR_DBP)	continue;

}

__STATIC_INLINE void disable_RTC_write_access(void)
{
	PWR->CR &=~PWR_CR_DBP;
	RCC->APB1ENR &=~ RCC_APB1ENR_PWREN;
}

__STATIC_INLINE void enable_rtc_init_state(void)
{
  /* RTC init mode */
  /* Configure RTC */
  /* (1) Write access for RTC registers */
  /* (2) Enable init phase */
  /* (3) Wait until it is allow to modify RTC register values */
		
  RTC->WPR = 0xCA; /* (1) */ 
  RTC->WPR = 0x53; /* (1) */
  RTC->ISR |= RTC_ISR_INIT; /* (2) */
  
	while((RTC->ISR & RTC_ISR_INITF)!=RTC_ISR_INITF) /* (3) */
  { 
    /* add time out here for a robust application */
  }
}

__STATIC_INLINE void disable_rtc_init_state(void)
{
	/* (6) Disable init phase */
  /* (7) Disable write access for RTC registers */
		
	RTC->ISR &=~ RTC_ISR_INIT; /* (6) */
  RTC->WPR = 0xFE; /* (7) */
  RTC->WPR = 0x64; /* (7) */
}

uint32_t IsRTC_Run(void);









#endif

