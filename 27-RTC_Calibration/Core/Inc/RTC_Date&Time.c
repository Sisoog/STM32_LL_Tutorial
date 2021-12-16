/**
  ******************************************************************************
  * @file		RTC_Date&Time.c
  ******************************************************************************
  */  
 
	#include "RTC_Date&Time.h"
 
	struct time_t
	{
	  uint8_t sec;
	  uint8_t min;
	  uint8_t hour;
	};

	struct time_t Time;
	struct time_t Alarm;

	struct date_t
	{
	  uint8_t month;
	  uint8_t day;
	  uint16_t year;
	};

	struct date_t Date;

	uint8_t EndOfMonth[12]= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	uint32_t TimeCounter = 0;
	
// if calibration is needed	
extern volatile uint16_t RTC_Cal_ppm;	
#include "stm32f1xx_ll_tim.h"
/********************/
/********///Functions:

	
	  // function for setting the value of RTC prescaler
	  
	  //* for using LSE with and without calibration recommended values are
	  //** 0x00007FFD (To set prescaler to 32766, we must write 32765) and
	  //*** 0x00007FFF (To set prescaler to 32768, we must write 32767) accordingly
//	  void RTC_SetPrescaler(uint32_t Pre) 
//	  {									  
//	  RTC_InitStruct.AsynchPrescaler = Pre;
//	  LL_RTC_Init(RTC, &RTC_InitStruct);
//	  LL_RTC_SetAsynchPrescaler(RTC, Pre);
//	  }

	  // function for setting date
	  void DATE_Config( uint8_t fMonth,  uint8_t fDay, uint16_t fYear)
	  {
		Date.month = fMonth;
	  Date.day   = fDay;
	  Date.year  = fYear;
	  }

	  // function for updating date	
	void DATE_Update(void)
	{
	  if ((Time.hour == 00) & (Time.min == 00) & (Time.sec == 00))
	  {
		if(Date.day == EndOfMonth[Date.month -1])
		{
		  Date.day = 1U;
		  Date.month += 1U;
		}
		else
		{
		  Date.day = Date.day + 1;
		}
			/* calibration (every 24 hours) */
		  LL_TIM_EnableIT_CC1(TIM1);																	// to enabel tim1's capture interrupt
		  LL_RTC_CAL_SetCoarseDigital(BKP, (uint32_t) RTC_Cal_ppm);
			/* end of calibration */
		
			if (Date.month == 13)
			{
			Date.month = 1;
			Date.year += 1;
			}
	  }

	}

	  // function for updating time
	void TIME_Update(void)
	{
	  TimeCounter = LL_RTC_TIME_Get(RTC);
	  Time.hour = (TimeCounter/3600) % 24;
	  Time.min  = (TimeCounter % 3600) / 60;
	  Time.sec  = (TimeCounter % 3600) % 60;
	}

	  // function for setting time
	void TIME_Config(uint8_t fHour, uint8_t fMin, uint8_t fSec)
	{
	  Time.hour = fHour;
	  Time.min  = fMin;
	  Time.sec  = fSec;

	  LL_RTC_TIME_Set(RTC,((Time.hour * 3600) +
						   (Time.min * 60) +
							Time.sec));
	}

	  // function for setting alarm
	void ALARM_Config(uint8_t fHour, uint8_t fMin, uint8_t fSec)
	{
	  Alarm.hour = fHour;
	  Alarm.min  = fMin;
	  Alarm.sec  = fSec;

	  LL_RTC_ALARM_Set(RTC,((Alarm.hour * 3600) +
							(Alarm.min * 60) +
							 Alarm.sec));
	}

	  // function for printing date and time
	void Show_Date_and_Time(void)
	{
		for(int i=0; i<10; i++)
		{	
		TIME_Update();
		DATE_Update();
		printf("Time: %.2d:%.2d:%.2d\r\n", Time.hour, Time.min, Time.sec);
		LL_mDelay(998);
		}
		printf("Date: %.2d/%.2d/%.4d\r\n", Date.month, Date.day, Date.year);
    }
	
	  // must be used before changing the configurtion of RTC (also enables Alarm interrupt)
	void RTC_SettingBegin(bool IT)
	{
		LL_RTC_DisableWriteProtection(RTC);
		LL_RTC_EnterInitMode(RTC);
		if(IT)
		{
		LL_RTC_EnableIT_ALR(RTC);
		LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_17);
		LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_17);	
		}
	}

	  // must be used after changing the configurtion of RTC
	void RTC_SettingEnd(void)
	{
	LL_RTC_ExitInitMode(RTC);
	LL_RTC_EnableWriteProtection(RTC);	
	}		

/********************************* END OF FILE ********************************/
/******************************************************************************/


