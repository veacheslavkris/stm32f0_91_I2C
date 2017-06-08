 /* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "rtc_lse_32768.h"

#define RTC_TR_RESERVED_MASK    (0x007F7F7FU)
#define RTC_DR_RESERVED_MASK    (0x00FFFF3FU) 

#define HOURS			1
#define MINUTES		2
#define SECONDS 	3
#define WEEKDAY		5
#define DAY 			7
#define MONTH 		8
#define YEAR 			9

#define IS_LSE_RDY		((RCC->BDCR & RCC_BDCR_LSERDY) == RCC_BDCR_LSERDY)
#define IS_RTC_INITS 	((RTC->ISR & RTC_ISR_INITS)==RTC_ISR_INITS)

uint8_t rtc_bcd2byte(uint8_t);
uint8_t rtc_byte2bcd(uint8_t);

void RtcLse32768_Init()
{
	enable_RTC_write_access();	
	
	RCC->BDCR |= RCC_BDCR_LSEON;

	/* Wait till LSE is ready */  
  while((RCC->BDCR & RCC_BDCR_LSERDY)!=RCC_BDCR_LSERDY)
  { 
    /* add time out here for a robust application */
  }
  
	RCC->BDCR = (RCC->BDCR & ~RCC_BDCR_RTCSEL) | RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSE;
	
	enable_rtc_init_state();	
	
	/* (4) set prescaler, 32768/128 => 256 Hz, 256Hz/256 => 1Hz */
	//	hrtc.Init.AsynchPrediv = 127 - 007F;
	//  hrtc.Init.SynchPrediv = 255  - 00FF;
  RTC->PRER = 0x007F00FF; /* (4) */
	
	disable_rtc_init_state();
	
	disable_RTC_write_access();
}
//

void RtcSetDateTime(uint8_t* ary_bcd)
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

	enable_RTC_write_access();

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

	disable_RTC_write_access();
}
//

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
//

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
//

uint8_t rtc_bcd2byte(uint8_t Value)
{
  uint32_t tmp = 0U;
  tmp = ((uint8_t)(Value & (uint8_t)0xF0U) >> (uint8_t)0x4U) * 10U;
  return (tmp + (Value & (uint8_t)0x0FU));
}
//

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
//

uint32_t IsRTC_Run(void)
{
	if(IS_LSE_RDY && IS_RTC_INITS)
	{
		return 1;
	}
	else return 0;

}
//


