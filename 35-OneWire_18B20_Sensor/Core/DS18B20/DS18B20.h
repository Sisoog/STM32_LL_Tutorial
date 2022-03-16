/**
  ******************************************************************************
  * @file		DS18B20.h
  ******************************************************************************
  */
 
/********************/
/********///includes: 
 
	#include <stdbool.h> 
	#include "onewire.h"
	#include "DS18B20Config.h"

/********************/
/********///definitions:
/* Every onewire chip has different ROM code, but all the same chips has same family code */
/* in case of DS18B20 this is 0x28 and this is first byte of ROM address */
#define DS18B20_FAMILY_CODE						0x28
#define DS18B20_CMD_ALARMSEARCH				0xEC

/* Bits locations for resolution */
#define DS18B20_RESOLUTION_R1					6
#define DS18B20_RESOLUTION_R0					5

/* CRC enabled */
#ifdef DS18B20_USE_CRC	
#define DS18B20_DATA_LEN							9
#else
#define DS18B20_DATA_LEN							2
#endif


/***********************  DS18B20 Function Command Set ***********************/
#define DS18B20_CMD_CONVERTTEMP		0x44 	/* Convert temperature */

/********************/
/********///variables:


/********************/
/********///Functions:  
  
void DS18B20_Init(void);

float	DS18B20_Convert(void);
/********************************* END OF FILE ********************************/
/******************************************************************************/ 

