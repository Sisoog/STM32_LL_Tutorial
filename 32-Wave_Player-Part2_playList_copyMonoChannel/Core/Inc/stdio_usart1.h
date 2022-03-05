/**
  ******************************************************************************
  * @file		stdio_usart1.h
  ******************************************************************************
  */  
	 #include "stdint.h"
	 #include "stm32f1xx_ll_usart.h"

/********************/
/********///Functions:
	
	/* Function for transmitting 8bit data via USART */
	void write_uart(char);		

	/* Retargeting stdout_putchar as to use USART_TX for data output */	
	int stdout_putchar (int); 
  
  

/********************************* END OF FILE ********************************/
/******************************************************************************/

