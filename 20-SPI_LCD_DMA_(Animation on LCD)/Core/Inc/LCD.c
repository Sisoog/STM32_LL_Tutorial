/**
  ******************************************************************************
  * @file		delay.c
  ******************************************************************************
  */

#include "font.h"
#include "LCD.h"

#define X_NOP()		{__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();}
#define Y_NOP()		X_NOP();X_NOP();X_NOP();X_NOP();X_NOP();X_NOP();X_NOP();X_NOP();X_NOP();X_NOP();
#define Z_NOP()		Y_NOP();Y_NOP();Y_NOP();Y_NOP();Y_NOP();Y_NOP();Y_NOP();Y_NOP();Y_NOP();Y_NOP();

/// send 8 bit data to SPI
void send_SPI(uint8_t data)
{
		  while(!LL_SPI_IsActiveFlag_TXE(SPI1));
		  LL_SPI_TransmitData8(SPI1, data);
//			while(!LL_SPI_IsActiveFlag_RXNE(SPI1))
//			{}
}	

/// send command to the LCD
void LCD_SendCmd(uint8_t cmd)
{
	LL_GPIO_ResetOutputPin(LCD_GPIO, CE_pin); //put CE to Low state
//  Z_NOP()
	LL_GPIO_ResetOutputPin(LCD_GPIO, DC_pin); //LCD Command Mode
	
	
	send_SPI(cmd); //X address	 
  Z_NOP();Z_NOP();Z_NOP();Z_NOP();Z_NOP();Z_NOP();Z_NOP();Z_NOP();
	LL_GPIO_SetOutputPin(LCD_GPIO, DC_pin); //LCD exit from Command Mode
	LL_GPIO_SetOutputPin(LCD_GPIO, CE_pin); //put CE to High state
}

/// send data to the LCD
void LCD_SendData(uint8_t data)
{
	LL_GPIO_SetOutputPin(LCD_GPIO, DC_pin); //LCD Data Mode
	LL_GPIO_ResetOutputPin(LCD_GPIO, CE_pin); //put CE to Low state
	send_SPI(data); 
  X_NOP();X_NOP();X_NOP();X_NOP();Z_NOP();
	LL_GPIO_SetOutputPin(LCD_GPIO, CE_pin); //put CE to High state

}

/// LCD initialization
void LCD_init(void)
{
	LL_GPIO_SetOutputPin(LCD_GPIO, CE_pin); //LCD Enable

	
		////configure LCD :
	LL_GPIO_ResetOutputPin(LCD_GPIO, Reset_pin);
	LL_mDelay(10);
	LL_GPIO_SetOutputPin(LCD_GPIO, Reset_pin);
	LL_mDelay(10);

	///
	LCD_SendCmd(0x20);	//Function set(extended instruction set)
	LCD_SendCmd(0x90); //VOP
	LCD_SendCmd(0x07); //temperature coefficient
	LCD_SendCmd(0x13); //Voltage bias
	
	///
	LCD_SendCmd(0x20);	//Function set(basic instruction set)
	LCD_SendCmd(0x0C); //Display Configuration

	LCD_SendCmd(0x40);	//Y address
	LCD_SendCmd(0x80); //X address
}



/// x&y position
void LCD_goto_xy(int x, int y)
{
	uint8_t xh = 0x80 | (x & 0x7F);
	uint8_t yh = 0x40 | (y & 0x07);
	
	LCD_SendCmd(xh);
	LCD_SendCmd(yh);
}

/// clear screen
void LCD_cls(void)
{
	LCD_goto_xy(0,0);
	
	
	for(int i=0;i<=(48*84/8);i++)
	{
			LCD_SendData(0x00);
	}
	
}

/// send single character to the LCD
void LCD_send_char (char ch)
{
		ch = ch - ' ';
		int index = ch * 5;
		
		
		for(int i=0;i<5;i++)
		{
			LCD_SendData(font5x8[index++]);
		}
		LCD_SendData(0x00);
		
		
}

/// send string to the LCD
void LCD_puts (char *ch) 
{
	while(*ch)
	{
		LCD_send_char(*ch++);
	}
}

/// stdout
int stdout_putchar (int ch) 
{
  LCD_send_char(ch);
  return (ch);
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
