/**
  ******************************************************************************
  * @file		WAV_Handler.h
  ******************************************************************************
  */  

		#include <string.h>
		#include <stdio.h>
		#include <stdint.h>
		#include <stdbool.h>
		#include <stdlib.h>
		#include "SD_Utility.h"
		#include "nokia1661_lcd_driver.h"
		#include "lcd_font5x7.h"	
		#include "waveplayer_LCD_Utility.h"
		#include "stm32f1xx_ll_dac.h"
		#include "stm32f1xx_ll_cortex.h"
		#include "stm32f1xx_ll_dma.h"
		#include "stm32f1xx_ll_tim.h"
		#include "stdio_usart1.h"
		#include "stm32f1xx_ll_gpio.h"
		#include "stm32f1xx_ll_utils.h"
		

		
		#define Debug 1						//for Debug mode: 1, for normal run: 0

		#define SystemCoreClock 72000000
		#define waveTimer	TIM2
		#define WavebufferLength 4096								//DMA will transfer buffer of size 1KB to DAC
		#define waveDAC_Channel1 LL_DAC_CHANNEL_1
		#define waveDAC_Channel2 LL_DAC_CHANNEL_2		
		#define waveDMA1	DMA2
		#define	waveDMA1_Channel LL_DMA_CHANNEL_3
		#define waveDMA2	DMA2
		#define	waveDMA2_Channel LL_DMA_CHANNEL_4
		#define IsPressedKey() 	(((LL_GPIO_ReadInputPort(GPIOC)) & (1<<0)) == 0)

/********************/
/********///WAV file structure:

typedef struct __attribute__((packed)){
	//The "RIFF" chunk descriptor
   uint8_t			ChunkID[4];
   uint32_t		ChunkSize;
   uint8_t  		Format[4];
    //The "fmt" sub-chunk
   uint8_t		Subchunk1ID[4];
   uint32_t		Subchunk1Size;
   uint16_t		AudioFormat;
   uint16_t		NumChannels;
   uint32_t		SampleRate;
   uint32_t		ByteRate;
   uint16_t		BlockAlign;
   uint16_t		BitsPerSample; 
}wave_header_t1; 

typedef struct __attribute__((packed)){
	//The "data" sub-chunk
	 uint8_t		Subchunk2ID[4];
	 uint32_t		Subchunk2Size; 
}wave_header_t2; 

typedef struct {
	 wave_header_t1 signatureHeader;
	 wave_header_t2 dataHeader;
}wave_header_t; 

/********************/
/********///Functions:

	// Initialization
		void wave_Init(void);

  //Handling wave file
	bool wave_open(FIL *, char *);
	bool wave_readHeader(FIL *, wave_header_t *);
	void wave_DMAConf(wave_header_t *, uint8_t *);
	void wave_TimerConf(uint32_t);
	void wave_DACConf(wave_header_t *, uint8_t *);
	void wave_start(void);
	void wave_IsEndofFile(FIL *, uint8_t *, uint16_t);
	void wave_play(char *);
	void wave_EndofFile_Callback(void);
	void wave_DMA_TC_Callback(void);
	void wave_DMA_HT_Callback(void);	
	FRESULT scan_files (char*); 		/* Start node to be scanned (***also used as work area***) */
/********************************* END OF FILE ********************************/
/******************************************************************************/

