/**
  ******************************************************************************
  * @file		servo.c
  ******************************************************************************
  */
 
/********************/
/********///includes: 
  
	#include "servo.h"

/********************/
/********///definitions:


/********************/
/********///variables:


/********************/
/********///Functions:  
	
	/* Function for initializing servo */
		void servo_init(void)
		{
			LL_TIM_EnableCounter(TIM1);																								//Enable Timer1's counter
			LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);												//Enable channel1 of Timer1(PWM)
			LL_TIM_EnableAllOutputs(TIM1);																						//Enable Timer1's outputs
		}

//=========================================================		
	/* Function for moving servo to desired position */
		bool servo_moveto(int degree)
		{
			if((0 > degree) || (Angle_Range < degree))
			{
				return false;
			}
			
			uint32_t ARR = LL_TIM_GetAutoReload(TIM1) + 1;
			uint32_t CRR = (((( (float) degree / Angle_Range) * Pulse_Width_Range) + Min_Pulse_Width) / PWM_period) * ARR;
			LL_TIM_OC_SetCompareCH1(TIM1, CRR);	//Set Duty cycle 
			return true;
		}
		
//=========================================================		
	/* Function for sweeping from angle specified with begin_pos to the angle end_pos */		
		void servo_sweep(int begin_pos, int end_pos)
		{			
			if (begin_pos < 0)
			{
				begin_pos = 0;
			}
			
			if (begin_pos > 180)
			{
				begin_pos = 180;
			}

			if (end_pos < 0)
			{
				end_pos = 0;
			}
			
			if (end_pos > 180)
			{
				end_pos = 180;
			}
			
			if (end_pos < begin_pos)
			{
				servo_moveto(begin_pos);
				return;
			}
			
			
			for (int i = end_pos; i > begin_pos; i--)
			{
				servo_moveto(i); 
				Delay_msec(30);
			}
			for (int i = begin_pos; i < end_pos; i++)
			{
				servo_moveto(i);
				Delay_msec(30);
			}
		}

/********************************* END OF FILE ********************************/
/******************************************************************************/ 




