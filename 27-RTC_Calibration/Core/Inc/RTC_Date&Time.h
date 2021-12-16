/**
  ******************************************************************************
  * @file		RTC_Date&Time.h
  ******************************************************************************
  */
  #include "stdint.h"
  #include "stdbool.h"
	#include "stm32f1xx_ll_rtc.h"
	#include "stm32f1xx_ll_exti.h"
	#include "stdio.h"
	#include "stm32f1xx_ll_utils.h"

//#if defined(RTC_InitStruct)
//extern LL_RTC_InitTypeDef RTC_InitStruct;
//#endif 

//#ifndef RTC_InitStruct
//LL_RTC_InitTypeDef RTC_InitStruct = {0};
//#endif
	
/********************/
/********///Functions:
	
//	void RTC_SetPrescaler(uint32_t);				  		  // function for setting the value of RTC prescaler
	void DATE_Update(void);									  // function for updating date
	void TIME_Update(void);									  // function for updating time
	void DATE_Config(uint8_t, uint8_t, uint16_t);			  // function for setting date
	void TIME_Config(uint8_t, uint8_t, uint8_t);		      // function for setting time
	void ALARM_Config(uint8_t, uint8_t, uint8_t);			  // function for setting alarm
	void Show_Date_and_Time(void);								  // function for printing date and time
	void RTC_SettingBegin(bool);							  // must be used before changing the configurtion of RTC (also enables alarm interrupt)
	void RTC_SettingEnd(void);								  // must be used after changing the configurtion of RTC

/********************************* END OF FILE ********************************/
/******************************************************************************/
