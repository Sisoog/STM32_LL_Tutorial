/**
  ******************************************************************************
  * @file		servo_config.h
  ******************************************************************************
  */

#ifndef servo_config_h 
#define servo_config_h 
/********************/
/********///includes: 
  
	#include "stm32f1xx_ll_utils.h"
	#include "stm32f1xx_ll_tim.h"
/********************/
/********///definitions:
//servo's angle range in Degrees
#define Angle_Range 				180
// Minimum pulse width in microsecends that corresponds to servo's 0 degree postion
#define Min_Pulse_Width 		550		
// Maxmum pulse width in microsecends that corresponds to servo's 180 degree postion
#define Max_Pulse_Width 		2800	
// Range of pulse width in microsecends that corresponds to servo's angle range
#define Pulse_Width_Range 	(Max_Pulse_Width -  Min_Pulse_Width)
//Appropriate PWM wave period in microsecends
#define PWM_period					20000
	


/********///function definitions:

#define Delay_msec(x)	LL_mDelay(x);


/***********************  Commands ***********************/
	
/********************/
/********///variables:


/********************/
/********///Functions:  


/********************************* END OF FILE ********************************/
/******************************************************************************/ 
#endif
