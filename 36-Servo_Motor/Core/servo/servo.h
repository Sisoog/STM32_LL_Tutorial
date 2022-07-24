/**
  ******************************************************************************
  * @file		servo.h
  ******************************************************************************
  */

#ifndef servo_h 
#define servo_h
/********************/
/********///includes: 
  
	#include <stdbool.h>
	#include "servo_config.h"

/********************/
/********///definitions:

	
/********************/
/********///variables:


/********************/
/********///Functions:  

	/* Function for initializing servo */
	void servo_init(void);
	
	/* Function for moving servo to desired position */
	bool servo_moveto(int);

	/* Function for sweeping from angle specified with begin_pos to the angle end_pos */
	void servo_sweep(int, int);

/********************************* END OF FILE ********************************/
/******************************************************************************/ 
#endif

