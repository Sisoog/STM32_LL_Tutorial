/**
  ******************************************************************************
  * @file		SD_Utility.h
  ******************************************************************************
  */
	#include <stdio.h>
	#include "stm32f1xx_ll_cortex.h"
	#include "ff.h"
	#include "diskio.h"
	
	#define Debug_SDInit 0						//for Debug mode: 1, for normal run: 0	
/********************/
/********///Functions:
	void disk_timerproc (void);
	
	void SD_Card_Init(void);
	void Print_SD_Card_space(void);
	void list_file (/* DIR,*/  char *); //     DIR parameter is an Directory object


/********************************* END OF FILE ********************************/
/******************************************************************************/  
