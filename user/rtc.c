 /* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "rtc.h"

#define RTC_TR_RESERVED_MASK    (0x007F7F7FU)
#define RTC_DR_RESERVED_MASK    (0x00FFFF3FU) 

#define HOURS			1
#define MINUTES		2
#define SECONDS 	3
#define WEEKDAY		5
#define DAY 			7
#define MONTH 		8
#define YEAR 			9





void turn_rtc_lse_on(void);
void enable_rtc_init_state(void);
void disable_rtc_init_state(void);
void set_rtc_prediv_lse(void);
uint8_t rtc_bcd2byte(uint8_t);
void set_bcd_date_time(uint8_t*);
uint8_t rtc_byte2bcd(uint8_t);

void RtcInitLse()
{
	turn_rtc_lse_on();
	set_rtc_prediv_lse();
}

__INLINE void turn_rtc_lse_on(void)
{
  /* Enable the peripheral clock RTC */
  /* (1) Enable the LSE (LSI) */
  /* (2) Wait while it is not ready */
  /* (3) Enable PWR clock */
  /* (4) Enable write in RTC domain control register */
  /* (5) LSE (LSI) for RTC clock */
  /* (6) Disable PWR clock */
  
	RCC->APB1ENR |= RCC_APB1ENR_PWREN; /* (3) */
	
	/* Enable write access to Backup domain */
	PWR->CR |= PWR_CR_DBP; /* (4) */

	/* Wait for Backup domain Write protection disable */
	while((PWR->CR & PWR_CR_DBP)!=PWR_CR_DBP)	continue;
	
	RCC->BDCR |= RCC_BDCR_LSEON; /* (1) */

  while((RCC->BDCR & RCC_BDCR_LSERDY)!=RCC_BDCR_LSERDY) /* (2) */
  { 
    /* add time out here for a robust application */
  }
  	
  /* Set the new LSE configuration SET_BIT(RCC->BDCR, RCC_BDCR_LSEON) ----------------------*/
	RCC->BDCR = (RCC->BDCR & ~RCC_BDCR_RTCSEL) | RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSE; /* (5) */

	/* Wait till LSE is ready */  
	while((RCC->BDCR & RCC_BDCR_LSERDY)!=RCC_BDCR_LSERDY)continue;
	
	/* Disable write access to Backup domain */
//	PWR->CR &=~PWR_CR_DBP; /* (4) */
	
	/* Require to disable power clock if necessary */
  RCC->APB1ENR &=~ RCC_APB1ENR_PWREN; /* (7) */

}

void RtcSetDateTime(uint8_t* ary_bcd)
{
	set_bcd_date_time(ary_bcd);
}

void set_bcd_date_time(uint8_t* ary_bcd)
{
	uint32_t tmpreg;
	uint32_t datetmpreg;
	
	/* Date Time Structure from PC
	
	byte[] ary_date_time = new byte[11];

	ary_date_time[0] = (byte)'$';
	ary_date_time[1] = Hours;          // hours
	ary_date_time[2] = Minutes;           // minutes
	ary_date_time[3] = Seconds;           // seconds
	ary_date_time[4] = (byte)'w';   
	ary_date_time[5] = WeekDay;           // weekday
	ary_date_time[6] = (byte)'%';
	ary_date_time[7] = Day;           // Month
	ary_date_time[8] = Month;           // Day
	ary_date_time[9] = Year;           // Year
	ary_date_time[10] = (byte)'!';

	*/



	enable_rtc_init_state();

	RTC->CR&= ~RTC_CR_FMT; // set 24h mode
	
	tmpreg = (uint32_t)(((uint32_t)rtc_byte2bcd(ary_bcd[HOURS]) << 16U) | \
                        ((uint32_t)rtc_byte2bcd(ary_bcd[MINUTES]) << 8U) | \
                        ((uint32_t)rtc_byte2bcd(ary_bcd[SECONDS])));
                        
  datetmpreg = (((uint32_t)rtc_byte2bcd(ary_bcd[YEAR]) << 16U) | \
                 ((uint32_t)rtc_byte2bcd(ary_bcd[MONTH]) << 8U) | \
                 ((uint32_t)rtc_byte2bcd(ary_bcd[DAY])) | \
                 ((uint32_t)ary_bcd[WEEKDAY] << 13U)); 

	/* Set the RTC_TR register */
	RTC->TR = (uint32_t)(tmpreg & RTC_TR_RESERVED_MASK);
	
	/* Set the RTC_DR register */
  RTC->DR = (uint32_t)(datetmpreg & RTC_DR_RESERVED_MASK);

	disable_rtc_init_state();
}

__INLINE void set_rtc_prediv_lse(void)
{
	enable_rtc_init_state();

	/* (4) set prescaler, 32768/128 => 256 Hz, 256Hz/256 => 1Hz */
	//	hrtc.Init.AsynchPrediv = 127 - 007F;
	//  hrtc.Init.SynchPrediv = 255  - 00FF;
  RTC->PRER = 0x007F00FF; /* (4) */
	
	disable_rtc_init_state();
}

__INLINE void enable_rtc_init_state(void)
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

__INLINE void disable_rtc_init_state(void)
{
	/* (6) Disable init phase */
  /* (7) Disable write access for RTC registers */
		
	RTC->ISR &=~ RTC_ISR_INIT; /* (6) */
  RTC->WPR = 0xFE; /* (7) */
  RTC->WPR = 0x64; /* (7) */
}

void RtcGetCurDateTime_BIN(RTC_TimeTypeDef* sttTime, RTC_DateTypeDef* sttDate)
{
	uint32_t tmpreg = 0;
	uint32_t datetmpreg = 0;

	/* Clear RSF flag */
	RTC->ISR &= ~(uint32_t)RTC_ISR_RSF;
	
	while((RTC->ISR & RTC_ISR_RSF)!=RTC_ISR_RSF) continue;
			
	/* Get the TR register */
	tmpreg = (uint32_t)(RTC->TR & RTC_TR_RESERVED_MASK); 
	
	/* Fill the structure fields with the read parameters */
	sttTime->Hours = (uint8_t)((tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> 16U);
	sttTime->Minutes = (uint8_t)((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >>8U);
	sttTime->Seconds = (uint8_t)(tmpreg & (RTC_TR_ST | RTC_TR_SU));
	
	/* Convert the time structure parameters to Binary format */
	sttTime->Hours = (uint8_t)rtc_bcd2byte(sttTime->Hours);
	sttTime->Minutes = (uint8_t)rtc_bcd2byte(sttTime->Minutes);
	sttTime->Seconds = (uint8_t)rtc_bcd2byte(sttTime->Seconds);  
	
	/* Get the DR register */
	datetmpreg = (uint32_t)(RTC->DR & RTC_DR_RESERVED_MASK); 

	/* Fill the structure fields with the read parameters */
	sttDate->Year = (uint8_t)((datetmpreg & (RTC_DR_YT | RTC_DR_YU)) >> 16U);
	sttDate->Month = (uint8_t)((datetmpreg & (RTC_DR_MT | RTC_DR_MU)) >> 8U);
	sttDate->Date = (uint8_t)(datetmpreg & (RTC_DR_DT | RTC_DR_DU));
 
	/* Convert the date structure parameters to Binary format */
	sttDate->Year = (uint8_t)rtc_bcd2byte(sttDate->Year);
	sttDate->Month = (uint8_t)rtc_bcd2byte(sttDate->Month);
	sttDate->Date = (uint8_t)rtc_bcd2byte(sttDate->Date);  
}

void RtcGetCurDateTime_BCD(RTC_BCD_TimeTypeDef* sttTime, RTC_BCD_DateTypeDef* sttDate)
{
	uint32_t tmpreg = 0;
	uint32_t datetmpreg = 0;
	
	/* Clear RSF flag */
	RTC->ISR &= ~(uint32_t)RTC_ISR_RSF;
	
	while((RTC->ISR & RTC_ISR_RSF)!=RTC_ISR_RSF) continue;
			
	/* Get the TR register */
	tmpreg = (uint32_t)(RTC->TR & RTC_TR_RESERVED_MASK); 
	
	/* Fill the structure fields with the read parameters */
	sttTime->HoursT = (uint8_t)((tmpreg & (RTC_TR_HT)) >> 20U);
	sttTime->HoursU = (uint8_t)((tmpreg & (RTC_TR_HU)) >> 16U);
	
	sttTime->MinutesT = (uint8_t)((tmpreg & (RTC_TR_MNT)) >>12U);
	sttTime->MinutesU = (uint8_t)((tmpreg & ( RTC_TR_MNU)) >>8U);

	sttTime->SecondsT = (uint8_t)((tmpreg & (RTC_TR_ST)) >>4U);
	sttTime->SecondsU = (uint8_t)(tmpreg & (RTC_TR_SU));
	
	/* Get the DR register */
	datetmpreg = (uint32_t)(RTC->DR & RTC_DR_RESERVED_MASK); 

	/* Fill the structure fields with the read parameters */
	sttDate->YearT = (uint8_t)((datetmpreg & (RTC_DR_YT)) >> 20U);
	sttDate->YearU = (uint8_t)((datetmpreg & (RTC_DR_YU)) >> 16U);
	
	if((datetmpreg & (RTC_DR_MT)) == RTC_DR_MT)
	{
		sttDate->MonthT = 1;
	}
	else sttDate->MonthT=0;
	
	sttDate->MonthU = (uint8_t)((datetmpreg & (RTC_DR_MU)) >> 8U);
	
	sttDate->DateT = (uint8_t)((datetmpreg & (RTC_DR_DT)) >> 4U);
	sttDate->DateU = (uint8_t)(datetmpreg & (RTC_DR_DU));

	sttDate->WeekDayU = (uint8_t)((datetmpreg & (RTC_DR_WDU)) >> 13U); 

}

uint8_t rtc_bcd2byte(uint8_t Value)
{
  uint32_t tmp = 0U;
  tmp = ((uint8_t)(Value & (uint8_t)0xF0U) >> (uint8_t)0x4U) * 10U;
  return (tmp + (Value & (uint8_t)0x0FU));
}

uint8_t rtc_byte2bcd(uint8_t Value)
{
  uint32_t bcdhigh = 0U;
  
  while(Value >= 10U)
  {
    bcdhigh++;
    Value -= 10U;
  }
  
  return  ((uint8_t)(bcdhigh << 4U) | Value);
}

