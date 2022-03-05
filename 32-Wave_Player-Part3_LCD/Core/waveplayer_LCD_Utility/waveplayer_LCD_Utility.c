/**
  ******************************************************************************
  * @file		waveplayer_LCD_Utility.c
  ******************************************************************************
  */

	#include "waveplayer_LCD_Utility.h"

/********************/
/********///Functions:

	void delay_ms(uint32_t x)
	{
		LL_mDelay(x);
	}

	/* 16 bits array */
		void LCD_Show_image(uint8_t *header, uint16_t *image)
	{
		uint8_t x_offset = header[0];
		uint8_t y_offset = header[1];
		uint16_t sizex = header[2];
		uint16_t sizey = header[3];
		uint16_t color16bit = 0;
		
		for(uint8_t j = 0; j <= sizey; j++)	
			for(uint8_t i = 0; i <= sizex; i++)
			{
				color16bit = image[j * sizex + i];
				nlcdPixel(x_offset + i, y_offset + j, color16bit);
			}
	}
	
	
	/* 8 bits array */
//	void LCD_Show_image(uint8_t *header, uint8_t *image)
//	{
//		uint8_t x_offset = header[0];
//		uint8_t y_offset = header[1];
//		uint16_t sizex = header[2];
//		uint16_t sizey = header[3];
//		uint16_t color16bit = 0;
//		
//		for(uint8_t j = 0; j <= sizey; j+= 1)	
//			for(uint8_t i = 0; i <= sizex * 2; i+= 2)
//			{
//				color16bit = (image[j * sizex + i] << 8) | (image[j * sizex + i + 1]);
//				nlcdPixel(x_offset + (i / 2), y_offset + j, color16bit);
//			}
//	}

/********************************* END OF FILE ********************************/
/******************************************************************************/  

