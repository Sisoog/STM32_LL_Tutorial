/**
  ******************************************************************************
  * @file		WAV_Handler.c
  ******************************************************************************
  */  
		#include "WAV_Handler.h"
		#include "waveplayer_LCD_Wallpaper.h"
/********************/
/********///variables:

	uint8_t headerBuffer[60];						// to read the header	
	uint8_t wavBuffer[WavebufferLength];		  	// DMA buffer

	volatile uint8_t BufferAction = 0;				// indicator to what buffer action is needed
	volatile bool EndofFile = false;				// indicator of the end of the file
	volatile bool stop,next = false;
	
	/* LCD interface */
	uint16_t elapsedHalfBuffers;							//number of half_buffers that have been played
	volatile uint16_t counter = 0;
/********************/
/********///Functions:
	
	/*********************** Initialization ***********************/	
	void wave_Init()
	{
		#if(Debug == 1)
		printf("Initializing wave player\n\r");	
		#endif
		
		SD_Card_Init();
		LL_DAC_Enable(DAC, waveDAC_Channel1);			//Enable DAC channel 1
		LL_DAC_Enable(DAC, waveDAC_Channel2);			//Enable DAC channel 2
		
		#if(Debug == 1)
		Print_SD_Card_space();
		#endif
	}

	/********************* opening and reading the WAV file *********************/
	//opening the file
	bool wave_open(FIL *filptr, char *fileName)
	{
		FRESULT fr;     /* FatFs return code */
		fr = f_open(filptr, fileName, FA_READ);
		
		if(fr)
		{
			#if(Debug == 1)
			printf("Can not open the file\r\n -> %d \r\n", fr);
			#endif
			return false;
		}

		#if(Debug == 1)
		printf("Successfully opened the file\r\n");
		#endif
		return true;

	}

	//reading and parsing the Header of the WAV file	
	bool wave_readHeader(FIL *filptr, wave_header_t *wav1HeaderPtr)
	{
		FRESULT fr;     /* FatFs return code */
		UINT br;		//file read
		fr = f_read(filptr, headerBuffer, sizeof(headerBuffer), &br);

		if(fr)
		{
			#if(Debug == 1)
			printf("Can not read header of the file\r\n -> %d \r\n", fr);
			#endif
			return false;
		}
		

		#if(Debug == 1)
		printf("reading the file..\r\n");
		#endif
		//setting the wav1's Header variables:
		//The "RIFF" chunk and The "fmt" sub-chunk descriptors
		memcpy(&wav1HeaderPtr->signatureHeader, headerBuffer, sizeof(wave_header_t1));
		//The "data" sub-chunk
		memcpy(&wav1HeaderPtr->dataHeader,
					 &headerBuffer[wav1HeaderPtr->signatureHeader.Subchunk1Size + 16 + 4],
					 sizeof(wave_header_t2));

		if( (strncmp(wav1HeaderPtr->signatureHeader.ChunkID, "RIFF", 4) != 0) ||  
				(strncmp(wav1HeaderPtr->signatureHeader.Format , "WAVE", 4) != 0) )
		{
			return false;
		}

		
		// set read/write pointer after the header information to read data 
		uint16_t headerEnd = 16 + wav1HeaderPtr->signatureHeader.Subchunk1Size + 12;		// pointer to the end of header of wav file	
		f_lseek(filptr, headerEnd);
		f_read(filptr, wavBuffer, WavebufferLength, &br);
	
		/**** print properties of the wav file, extracted from the header ****/
		#if(Debug == 1)
		printf("number of bytes in the file(header excluded): %x\r\n", wav1HeaderPtr->dataHeader.Subchunk2Size);
		printf("number of channels: %d\r\n", wav1HeaderPtr->signatureHeader.NumChannels);
		printf("sample rate: %d\r\n", wav1HeaderPtr->signatureHeader.SampleRate);
		printf("byte rate: %d\r\n", wav1HeaderPtr->signatureHeader.ByteRate);
		printf("bits per sample: %d\r\n", wav1HeaderPtr->signatureHeader.BitsPerSample);
		printf("data ID: %c%c%c%c\r\n", wav1HeaderPtr->dataHeader.Subchunk2ID[0],
					 wav1HeaderPtr->dataHeader.Subchunk2ID[1],
					 wav1HeaderPtr->dataHeader.Subchunk2ID[2],
					 wav1HeaderPtr->dataHeader.Subchunk2ID[3]);
		#endif
		return true;

	}

	/********************* Handling the playing of the WAV file *********************/	
	void wave_DMAConf(wave_header_t *wav1HeaderPtr, uint8_t *wavBuffer)
	{
				LL_DMA_DisableChannel(waveDMA1, waveDMA1_Channel);  //Disable DMA before configuraton, for consecutive  playing
				/******************* Determine DMA transfer data length, addrees of DAC register and 
				* memory/peripheral data alignment, based on bits per sample & Number of channels */
				volatile uint32_t *DACRegADS = &DAC1->DHR8R1;
				volatile uint32_t *DAC2RegADS = &DAC1->DHR8R2;
				uint32_t DMA_DataLength = WavebufferLength;
				uint32_t waveBitPerSampleM, waveBitPerSampleP;	// bits per sample for setting Memory and Peripheral
		
				/* mono 8 bits per sample */
				if((wav1HeaderPtr->signatureHeader.BitsPerSample == 8) && (wav1HeaderPtr->signatureHeader.NumChannels == 1))
				{
					DACRegADS = &DAC1->DHR8R1;
					DAC2RegADS = &DAC1->DHR8R2;
					waveBitPerSampleM = LL_DMA_MDATAALIGN_BYTE;
					waveBitPerSampleP = LL_DMA_PDATAALIGN_BYTE;
					DMA_DataLength = WavebufferLength;
				}
				/* mono 16 bits per sample */
				else if((wav1HeaderPtr->signatureHeader.BitsPerSample == 16) && (wav1HeaderPtr->signatureHeader.NumChannels == 1))
				{
					DACRegADS = &DAC1->DHR12L1;
					DAC2RegADS = &DAC1->DHR12L2;
					waveBitPerSampleM = LL_DMA_MDATAALIGN_HALFWORD;
					waveBitPerSampleP = LL_DMA_PDATAALIGN_HALFWORD;
					DMA_DataLength = WavebufferLength / 2;
				}
				/* stereo 8 bits per sample */
				else if((wav1HeaderPtr->signatureHeader.BitsPerSample == 8) && (wav1HeaderPtr->signatureHeader.NumChannels == 2))
				{
					DACRegADS = &DAC1->DHR8RD;
					waveBitPerSampleM = LL_DMA_MDATAALIGN_HALFWORD;
					waveBitPerSampleP = LL_DMA_PDATAALIGN_HALFWORD;
					DMA_DataLength = WavebufferLength / 2;
				}
				/* stereo 16 bits per sample */
				else if((wav1HeaderPtr->signatureHeader.BitsPerSample == 16) && (wav1HeaderPtr->signatureHeader.NumChannels == 2))
				{
					DACRegADS = &DAC1->DHR12LD;
					waveBitPerSampleM = LL_DMA_MDATAALIGN_WORD;
					waveBitPerSampleP = LL_DMA_PDATAALIGN_WORD;
					DMA_DataLength = WavebufferLength / 4;
				}
				#if(Debug == 1)
				printf("DMA_DataLength: %d\r\n\n", DMA_DataLength);
				#endif
				
				//set DMA  Data width for peripheral and memory according to Bits per Sample of the file
				LL_DMA_SetPeriphSize(waveDMA1, waveDMA1_Channel, waveBitPerSampleP);
				LL_DMA_SetMemorySize(waveDMA1, waveDMA1_Channel, waveBitPerSampleM);	
				/********************** configuring DMA **********************/
				LL_DMA_ConfigAddresses(waveDMA1,
															 waveDMA1_Channel,
															 (uint32_t)wavBuffer,
															 (uint32_t) DACRegADS,
															 LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
												 
				LL_DMA_SetDataLength(waveDMA1, waveDMA1_Channel, 
														 DMA_DataLength);		
												 
				LL_DMA_EnableIT_TC(waveDMA1, waveDMA1_Channel);
				LL_DMA_EnableIT_HT(waveDMA1, waveDMA1_Channel);
				LL_DMA_EnableIT_TE(waveDMA1, waveDMA1_Channel);
				LL_DMA_EnableChannel(waveDMA1, waveDMA1_Channel);	
				
				if(wav1HeaderPtr->signatureHeader.NumChannels == 1)
				{
					LL_DMA_SetPeriphSize(waveDMA2, waveDMA2_Channel, waveBitPerSampleP);
					LL_DMA_SetMemorySize(waveDMA2, waveDMA2_Channel, waveBitPerSampleM);	
					/********************** configuring DMA **********************/
					LL_DMA_ConfigAddresses(waveDMA2,
																 waveDMA2_Channel,
																 (uint32_t)wavBuffer,
																 (uint32_t) DAC2RegADS,
																 LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
													 
					LL_DMA_SetDataLength(waveDMA2, waveDMA2_Channel, 
															 DMA_DataLength);		
				
					LL_DMA_EnableChannel(waveDMA2, waveDMA2_Channel);	
				}
	}
	
	
	/********************** Configuring wave timer **********************/	
	void wave_TimerConf(uint32_t SampleRate)		//set Timer update frequency according to SampleRate of the file
	{
		LL_TIM_DisableCounter(waveTimer);					//Disable Timer before configuraton, for consecutive  playing
		LL_TIM_InitTypeDef TIM_InitStruct = {0};

		TIM_InitStruct.Prescaler = (SystemCoreClock/1000000) - 1;
		TIM_InitStruct.Autoreload = (SystemCoreClock / ((TIM_InitStruct.Prescaler + 1) * SampleRate) ) - 1;
		LL_TIM_Init(waveTimer, &TIM_InitStruct);	

		LL_TIM_EnableUpdateEvent(waveTimer);	
	}
	
	void wave_DACConf(wave_header_t *wav1HeaderPtr, uint8_t *wavBuffer)
	{
		LL_DAC_Enable(DAC, waveDAC_Channel1);			//Enable DAC channel 1
		LL_DAC_Enable(DAC, waveDAC_Channel2);			//Enable DAC channel 2
		
		wave_DMAConf(wav1HeaderPtr, wavBuffer);
		wave_TimerConf(wav1HeaderPtr->signatureHeader.SampleRate);
		LL_DAC_EnableDMAReq(DAC, LL_DAC_CHANNEL_1);
		
		if(wav1HeaderPtr->signatureHeader.NumChannels == 1)
			{
			LL_DAC_EnableDMAReq(DAC, LL_DAC_CHANNEL_2);
			}
	}
	
	void wave_start(void)
	{
		#if(Debug == 1)
		printf("playing the file... \r\n");	
		#endif
		stop = false;
		next = false;
		EndofFile = false;
		elapsedHalfBuffers = 0;
		LL_TIM_EnableCounter(waveTimer);
	}
	
	void wave_IsEndofFile(FIL *filptr, uint8_t *wavBuffer, uint16_t BitsPerSample)
	{
		UINT br;		//file read
//		if(BufferAction == 0)

		
		if(BufferAction==3)						//if both of DMA HT and TC buffer action flags have risen before one of them is cleared
		{
			#if(Debug == 1)
			printf("Low reading speed!\r\n");
			#endif
		}
		
		if(BufferAction&0x1)					//in case of DMA HT buffer action flag have risen
		{
			elapsedHalfBuffers++;
			br = 0;
			f_read(filptr, wavBuffer, WavebufferLength / 2, &br);		//first half of the wavbuffer must be updated	
			
			if(BitsPerSample == 16)	//if bits per sample is 16 (else it's 8)
			{	
				for(uint16_t i = 1;  i<WavebufferLength/2;  i += 2)
				{
					wavBuffer[i] ^= 0x80;									//required for 16 bits per sample mode
				}
			}			
			BufferAction &=~0x1;				//Clear the buffer action flag
			
			if(br<WavebufferLength/2)
			{
				EndofFile = true;
			}
		}
		
		if(BufferAction&0x2)					//in case of DMA TC buffer action flag have risen
		{
			elapsedHalfBuffers++;
			br = 0;
			f_read(filptr, wavBuffer + (WavebufferLength / 2), WavebufferLength / 2, &br);	//second half of the wavbuffer must be updated	
			
			if(BitsPerSample == 16)		//if bits per sample is 16 (else it's 8)	
			{
				for(uint16_t i = WavebufferLength/2 + 1;  i<WavebufferLength;  i += 2)
				{
					wavBuffer[i] ^= 0x80;									//required for 16 bits per sample mode
				}
			}			
			BufferAction &=~0x2;				//Clear the buffer action flag
			
			if(br<WavebufferLength/2)			//to indicate end of the file
			{
				EndofFile = true;
			}
		}	
	}
	
	void wave_play(char *fileName)
	{

		FIL fil;      						    /* File object */
		/* wave handling variables */
		wave_header_t wavHeader1;						//struct a new header		
		
		if(!wave_open(&fil, fileName))
			{
				#if(Debug == 1)
				printf("Error in opening the file\r\n \r\n");
				#endif
				return;
			}
			
		bool isWAV = wave_readHeader(&fil, &wavHeader1);
		
		if(!isWAV)
			{
				#if(Debug == 1)
				printf("file is not in WAV format or can not read header of the file!\r\n");
				#endif
				return;
			}
			
		wave_DACConf(&wavHeader1, wavBuffer);	
		
		bool unpressedKey = false;
		/** LCD Graphics **/

		char play_time[6] = {0};
		char remaining_time[6] = {0}; 			
		char file_Duration[6] = {0};
		uint32_t timeElapsed = 0, timeRemaining = 0;
		
		#define BytesInFile		wavHeader1.dataHeader.Subchunk2Size
		#define ByteRate			wavHeader1.signatureHeader.ByteRate
		#define fileDuration	BytesInFile / ByteRate
		
		nlcdGotoCharXY(1,5);
		nlcdStringP(LCD_VGA_FUCHSIA, PSTR("Duration: "));
		sprintf(file_Duration, "%d:%02d", fileDuration/60, fileDuration%60);
		nlcdStringP(LCD_VGA_FUCHSIA, PSTR(file_Duration));
		
		LCD_Show_image(wallPaperHeader, (uint16_t*) wallPaper);
		
		/* Progrees Bar border */
		for(int i=0; i<120; i++)
		{
			nlcdPixel(5+i, 120, LCD_VGA_BLUE);
			nlcdPixel(5+i, 121, LCD_VGA_BLUE);
		}
		
		for(int i=0; i<16; i++)
		{
			nlcdPixel(5, 120+i, LCD_VGA_BLUE);
			nlcdPixel(6, 120+i, LCD_VGA_BLUE);
		}
	 
		for(int i=0; i<120; i++)
		{
			nlcdPixel(5+i, 120+15, LCD_VGA_BLUE);
			nlcdPixel(5+i, 121+15, LCD_VGA_BLUE);				
		}
	 
		for(int i=0; i<16; i++)
		{
			nlcdPixel(5+120, 120+i, LCD_VGA_BLUE);
			nlcdPixel(6+120, 120+i, LCD_VGA_BLUE);
		}
		
		/* start playing the wave file */
		wave_start();
		

		
		static UINT i = 0;
		
		while(!(stop || next))
		{
			if(counter >= 1000)
			{
					counter = 0;
					timeElapsed = (elapsedHalfBuffers * (WavebufferLength / 2)) / ByteRate;
					timeRemaining = fileDuration - timeElapsed;

					sprintf(play_time, "%d:%02d", timeElapsed/60, timeElapsed%60);
					sprintf(remaining_time, "%d:%02d", timeRemaining/60, timeRemaining%60);
				
					nlcdGotoCharXY(1,18);
					nlcdStringP(LCD_VGA_BLACK, PSTR(play_time));
					nlcdGotoCharXY(17,18);
					nlcdStringP(LCD_VGA_BLACK, PSTR(remaining_time));

					uint8_t playPercent = (uint8_t) 100 * 
					((float)(elapsedHalfBuffers * (WavebufferLength / 2)) / BytesInFile);

					for(; i < 116 * playPercent / 100; i++)
						{
							nlcdPixel(7+i, 125, LCD_VGA_BLACK);
							nlcdPixel(8+i, 126, LCD_VGA_BLACK);
							nlcdPixel(9+i, 127, LCD_VGA_BLACK);
							nlcdPixel(10+i,128, LCD_VGA_BLACK);
							nlcdPixel(9+i, 129, LCD_VGA_BLACK);
							nlcdPixel(8+i, 130, LCD_VGA_BLACK);
							nlcdPixel(7+i, 131, LCD_VGA_BLACK);
						}

			}
			
			wave_IsEndofFile(&fil, wavBuffer, wavHeader1.signatureHeader.BitsPerSample);
			
			//polling for button
			if(!unpressedKey && !IsPressedKey())
			{
				unpressedKey = true;
			}
			
			if(unpressedKey && IsPressedKey())
			{
				#if(Debug == 1)
				printf("next song!");
				#endif
				next = true;
			}
		}
		
		BufferAction = 0;
		LL_DMA_DisableChannel(waveDMA1, waveDMA1_Channel);
		LL_DMA_DisableChannel(waveDMA2, waveDMA2_Channel);			
		LL_TIM_DisableCounter(waveTimer);
		LL_DAC_Disable(DAC, waveDAC_Channel1);
		LL_DAC_Disable(DAC, waveDAC_Channel2);
		memset(&i, 0, sizeof(UINT));
	}
		

	void wave_EndofFile_Callback(void)
	{
		if(EndofFile)
		{
			LL_DMA_DisableChannel(waveDMA1, waveDMA1_Channel);
			BufferAction = 0;
			stop = true;
		}
	}

	void wave_DMA_TC_Callback(void)
	{
		BufferAction |= 0x2;
	}
	
	void wave_DMA_HT_Callback(void)	
	{
		BufferAction |= 0x1;		
	}
	
	
/**
  * @brief This function handles DMA2 channel3 global interrupt.
  */
void DMA2_Channel3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Channel3_IRQn 0 */
  if(LL_DMA_IsActiveFlag_TC3(DMA2) == 1)
  {
		
    /* Clear flag DMA transfer complete */
    LL_DMA_ClearFlag_TC3(DMA2);
		wave_DMA_TC_Callback();
	}
////// if transfer fails :	
	else if(LL_DMA_IsActiveFlag_TE3(DMA2) == 1)	
	{
			LL_DMA_ClearFlag_TE3(DMA2);
			LL_DMA_DisableChannel(DMA2,LL_DMA_CHANNEL_3);
  }
//////Half transfer complete	
  if(LL_DMA_IsActiveFlag_HT3(DMA2) == 1)
  {
    /* Clear flag DMA half transfer */
    LL_DMA_ClearFlag_HT3(DMA2);
		wave_DMA_HT_Callback();
	}
	
	wave_EndofFile_Callback();
	
  /* USER CODE END DMA2_Channel3_IRQn 0 */

  /* USER CODE BEGIN DMA2_Channel3_IRQn 1 */

  /* USER CODE END DMA2_Channel3_IRQn 1 */
}	


	/********************** scan for .wav files and play them **********************/	
FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
)
{	 
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;
		static char workPath[128] = {0};
		
		/* for displaying details on LCD */
		nlcdInit();
		nlcdSetBackgroundColor(LCD_VGA_GRAY);
		nlcdSetFont(font5x7latcyr);
		nlcdSetOrientation(LCD_ORIENTATION_180);
		/*****************************/
		
		strncpy(workPath, path, sizeof(workPath) - 1);
    res = f_opendir(&dir, workPath);                       /* Open the directory */
		
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(workPath);
                snprintf(&workPath[i],sizeof(workPath) - i - 1, "/%s", fno.fname);
                res = scan_files(workPath);                    /* Enter the directory */
                if (res != FR_OK) break;
                workPath[i] = 0;
            } 
						else 
						{                          									/* It is a file. */
									
									if(strstr(&fno.fname[strlen(fno.fname) - 5], ".WAV"))
									{
										char *filePath = (char*) malloc(sizeof(char) * 128);
										snprintf(filePath, 127, "%s/%s", workPath, fno.fname);
										#if(Debug == 1)	
										printf("we play: %s\n", filePath);
										#endif
										
										nlcdClear();
										nlcdGotoCharXY(1,1);
										nlcdStringP(LCD_VGA_BLACK, PSTR("Now playing: "));
										nlcdGotoCharXY(3,3);
										nlcdStringP(LCD_VGA_BLUE, PSTR(fno.fname));


										
										wave_play(filePath);
										free(filePath);	
									}
									
						}							
        }
						f_closedir(&dir);
			}
			return res;
}

//*********************************************************************************



/********************************* END OF FILE ********************************/
/******************************************************************************/

