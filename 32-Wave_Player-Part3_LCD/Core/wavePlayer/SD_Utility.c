/**
  ******************************************************************************
  * @file		SD_Utility.c
  ******************************************************************************
  */
	#include "SD_Utility.h"
/********************/
/********///variables:

	/* file handling variables */
	FATFS FatFs;							/* File system */
	FATFS *pfs; 
	extern volatile uint16_t counter;
/********************/
/********///Functions:  
  
	/*********************** Initialization ***********************/	
	void SD_Card_Init(void)
	{

		SysTick_Config(SystemCoreClock / 1000);
		LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
		LL_SYSTICK_EnableIT();
		
		int stat = 	f_mount(&FatFs, "", 0);
		#if(Debug_SDInit == 1)
		if(stat)
			printf("failed to mount the SD Card \r\n");
		else
			printf("successfully mounted the SD Card\r\n");
		#endif

		stat = disk_initialize(0);
		#if(Debug_SDInit == 1)
		printf("initialization status: %d\r\n", stat);
		#endif
	}

	/*********************** Card capacity details ***********************/	
	void Print_SD_Card_space(void)
	{		
	/* Check storage size and free space */
	float total, free_space;
	DWORD fre_clust;
	f_getfree("0:", &fre_clust, &pfs);
		
	total = (float)((pfs->n_fatent - 2) * pfs->csize  * 0.5  / (1024));
	printf("SD CARD Total Size: \t%.2f Mega Bytes\r\n", total);


	free_space = (float)(fre_clust * pfs->csize  * 0.5 / (1024));
	printf("SD CARD Free SPACE: \t%.2f Mega Bytes\r\n", free_space);
	}
	
	/************************* Search for files *************************/
	
void list_file (/* DIR dj,*/ char *pattern)
{
    FRESULT fr;     /* Return value */
    FILINFO fno;    /* File information */
		DIR dj;
	
	  fr = f_findfirst(&dj, &fno, "", pattern); /* Start to search for the specified files */
	
		if(fr != FR_OK)
			printf("didn't find any wav file! -> %d \r\n", fr);
		
    while (fr == FR_OK && fno.fname[0]) {         /* Repeat while an item is found */
        printf("%s\n", fno.fname);                /* Print the object name */
        fr = f_findnext(&dj, &fno);               /* Search for next item */
    }

    f_closedir(&dj);
}

void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
	disk_timerproc();
	counter++;
  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}
/********************************* END OF FILE ********************************/
/******************************************************************************/ 

