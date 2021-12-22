/**
  ******************************************************************************
  * @file		show_all_reset_sources.c
  ******************************************************************************
  */  
  
  #include "show_all_reset_sources.h"
	
	
/********************/
/********///Functions:

void show_reset_sources (void)
	{
	if(LL_RCC_IsActiveFlag_LPWRRST())
		printf("reset source was Low Power reset\r\n");
	else if(LL_RCC_IsActiveFlag_PORRST())
		printf("reset source was POR\r\n");		
	else if(LL_RCC_IsActiveFlag_SFTRST())
		printf("reset source was software reset\r\n");	
	else if (LL_RCC_IsActiveFlag_WWDGRST())
		printf("reset source was WWDG or pin\r\n");	
	else if (LL_RCC_IsActiveFlag_IWDGRST())
		printf("reset source was IWDG or pin\r\n");		
	else if(LL_RCC_IsActiveFlag_PINRST())
		printf("reset source was reset pin\r\n");
	
	
		printf("Reset sources:\r\nPIN: %d\r\nIWDG: %d\r\nWWDG: %d\r\nSFT: %d\r\nLPW: %d\r\nPOR: %d\r\n",
	LL_RCC_IsActiveFlag_PINRST(), LL_RCC_IsActiveFlag_IWDGRST(), LL_RCC_IsActiveFlag_WWDGRST(),
	LL_RCC_IsActiveFlag_SFTRST(), LL_RCC_IsActiveFlag_LPWRRST(), LL_RCC_IsActiveFlag_PORRST() ); 
	LL_RCC_ClearResetFlags();
//	LL_mDelay(4000);
  }
  
  
/********************************* END OF FILE ********************************/
/******************************************************************************/



