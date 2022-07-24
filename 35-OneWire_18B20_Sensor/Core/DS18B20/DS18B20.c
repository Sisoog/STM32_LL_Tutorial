/**
  ******************************************************************************
  * @file		DS18B20.c
  ******************************************************************************
  */
 
/********************/
/********///includes: 
	#include "DS18B20.h"
	
/********************/
/********///variables:
	OneWire_t OneWire;
	uint8_t	  OneWireDevices;

/********************/
/********///Functions:  
// function to initialize the DS18B20 module
void DS18B20_Init(void)
{
	OneWire_Init(&OneWire, _DS18B20_GPIO, _DS18B20_PIN);	
}
////////////////////////////////////////////////////////////////////////////

// function to start the temperature conversion via DS18B20 module
float	DS18B20_Convert(void)
{
	DS18B20_Init();
	uint8_t data[2];
		
	OneWire_WriteByte(&OneWire, ONEWIRE_CMD_SKIPROM);			//skip ROM command
	OneWire_WriteByte(&OneWire, DS18B20_CMD_CONVERTTEMP);	//start conversion command

	OneWire_Delay(800);																		//time interval necessary for conversion
	OneWire_Reset(&OneWire);

	OneWire_WriteByte(&OneWire, ONEWIRE_CMD_SKIPROM);
	OneWire_WriteByte(&OneWire, ONEWIRE_CMD_RSCRATCHPAD);	//Read Scratch pad of 18B20 to get temperature
	
	for (int i = 0; i < 2; i++) 
	{
		/* Read 2bytes of the converted temperature byte by byte */
		data[i] = OneWire_ReadByte(&OneWire);
	}

	/* scale the converted value regarding to the scaling factor */
	uint16_t fpTemperature = (((uint16_t) data[1]) << 11)
		| (((uint16_t) data[0]) << 3);

	return ((float) fpTemperature) * 0.0078125;
}
////////////////////////////////////////////////////////////////////////////

/********************************* END OF FILE ********************************/
/******************************************************************************/ 

