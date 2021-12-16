/**
  ******************************************************************************
  * @file		stdio_usart1.c
  ******************************************************************************
  */  
  
  #include "stdio_usart1.h"
	
	
/********************/
/********///Functions:
		
	
	/* Function for transmitting 8bit data via USART */
	void write_uart(char data)		
	{
		while(!LL_USART_IsActiveFlag_TXE(USART1));
		LL_USART_TransmitData8(USART1, (uint8_t)data);
	}
	
	/* Retargeting stdout_putchar as to use USART_TX for data output */	
	int stdout_putchar (int ch) {
	  write_uart(ch);
	  return (ch);
	}  
  
  
/********************************* END OF FILE ********************************/
/******************************************************************************/
    
	
