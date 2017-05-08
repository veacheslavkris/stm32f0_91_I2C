
#ifndef RTC_H
#define RTC_H
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

#endif

