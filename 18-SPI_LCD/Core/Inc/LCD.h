/**
  ******************************************************************************
  * @file		LCD.h
  ******************************************************************************
  */
#include <stdint.h>

#define Reset_pin LL_GPIO_PIN_3
#define DC_pin		LL_GPIO_PIN_4
#define CE_pin 		LL_GPIO_PIN_0
#define LCD_GPIO	GPIOA

/// SPI transmission
void send_SPI(uint8_t data);

/// send command to the LCD
void LCD_SendCmd(uint8_t cmd);

/// send data to the LCD
void LCD_SendData(uint8_t data);
	
/// LCD initialization
void LCD_init(void);

/// x&y position
void LCD_goto_xy(int x, int y);

/// clear screen
void LCD_cls(void);

/// send single character to the LCD
void LCD_send_char (char ch);

/// send string to the LCD
void LCD_puts (char *ch); 

/// stdout
int stdout_putchar (int ch); 

/********************************* END OF FILE ********************************/
/******************************************************************************/

