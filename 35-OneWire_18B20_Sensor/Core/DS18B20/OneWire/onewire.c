/**
  ******************************************************************************
  * @file		onewire.c
  ******************************************************************************
  */
/********************/
/********///includes: 
 
	#include "onewire.h"
/********************/
/********///definitions:


/********************/
/********///variables:


/********************/
/********///Functions:  
  	
// Delay(us) function used for creating the required one_wire timing singals
void OneWire_Delay(uint16_t time_us)
{
	LL_TIM_SetCounter(ONEWIRE_TIM, 0);
	while(LL_TIM_GetCounter(ONEWIRE_TIM) <= time_us);
}
///////////////////////////////////////////////////////////////////////

// function to set one_wire signal to low logic
void OneWire_Low(OneWire_t *gp)
{
	LL_GPIO_ResetOutputPin(gp->GPIOx, gp->GPIO_Pin);
}	
///////////////////////////////////////////////////////////////////////

// function to set one_wire signal to high logic
void OneWire_High(OneWire_t *gp)
{
	LL_GPIO_SetOutputPin(gp->GPIOx, gp->GPIO_Pin);
}	
///////////////////////////////////////////////////////////////////////

// function to set the one_wire pin as input
void OneWire_Input(OneWire_t *gp)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
		
	GPIO_InitStruct.Pin = gp->GPIO_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;

	LL_GPIO_Init(gp->GPIOx, &GPIO_InitStruct);
}
///////////////////////////////////////////////////////////////////////

// function to set the one_wire pin as output
void OneWire_Output(OneWire_t *gp)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
		
	GPIO_InitStruct.Pin = gp->GPIO_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;

	LL_GPIO_Init(gp->GPIOx, &GPIO_InitStruct);
}
///////////////////////////////////////////////////////////////////////

// function to initialize the one-wire communication
void OneWire_Init(OneWire_t* OneWireStruct, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin) 
{	
	LL_TIM_EnableCounter(ONEWIRE_TIM);

	OneWireStruct->GPIOx = GPIOx;
	OneWireStruct->GPIO_Pin = GPIO_Pin;
	OneWire_Output(OneWireStruct);
	OneWire_High(OneWireStruct);
	OneWire_Delay(1000);
	OneWire_Low(OneWireStruct);
	OneWire_Delay(1000);
	OneWire_High(OneWireStruct);
	OneWire_Delay(2000);
}
///////////////////////////////////////////////////////////////////////

// function to reset the one-wire communication
uint8_t OneWire_Reset(OneWire_t* OneWireStruct)
{
	uint8_t i;
	
	/* Line low, and wait 500us */
	OneWire_Low(OneWireStruct);
	OneWire_Output(OneWireStruct);
	OneWire_Delay(500);
	/* Release line and wait for 70us */
//	OneWire_Input(OneWireStruct);
		OneWire_High(OneWireStruct);
	OneWire_Delay(70);
	/* Check bit value */
	i = LL_GPIO_IsInputPinSet(OneWireStruct->GPIOx, OneWireStruct->GPIO_Pin);
	
	/* Delay for 430 us */
	OneWire_Delay(430);
	/* Return value of presence pulse, 0 = OK, 1 = ERROR */
	return i;
}
///////////////////////////////////////////////////////////////////////

// function to write 1-bit to the one-wire line
void OneWire_WriteBit(OneWire_t* OneWireStruct, uint8_t bit)
{
	if (bit) 
	{
		/* Set line low */
		OneWire_Low(OneWireStruct);
		OneWire_Output(OneWireStruct);
		OneWire_Delay(10);
		
		/* Bit high */
		OneWire_High(OneWireStruct);	
		/* Wait for 60 us and release the line */
		OneWire_Delay(55);
	} 
	else 
	{
		/* Set line low */
		
		OneWire_Low(OneWireStruct);
		OneWire_Output(OneWireStruct);		
		OneWire_Delay(65);
		
		/* Bit high */
//		OneWire_Input(OneWireStruct);
	OneWire_High(OneWireStruct);		
		/* Wait for 5 us and release the line */
		OneWire_Delay(5);

	}

}
///////////////////////////////////////////////////////////////////////

// function to read 1-bit of the one-wire line
uint8_t OneWire_ReadBit(OneWire_t* OneWireStruct) 
{
	uint8_t bit = 0;
	
	/* Line low */
	OneWire_Output(OneWireStruct);
	OneWire_Low(OneWireStruct);

	OneWire_Delay(2);
	
	/* Release line */
		OneWire_High(OneWireStruct);
	OneWire_Delay(10);
	
	/* Read line value */
	if (LL_GPIO_IsInputPinSet(OneWireStruct->GPIOx, OneWireStruct->GPIO_Pin)) 
	{
		/* Bit is HIGH */
		bit = 1;
	}
	
	/* Wait 50us to complete 60us period */
	OneWire_Delay(50);
	
	/* Return bit value */
	return bit;
}
///////////////////////////////////////////////////////////////////////

// function to write 1-byte to the one-wire line
void OneWire_WriteByte(OneWire_t* OneWireStruct, uint8_t byte) {
	uint8_t i = 8;
	/* Write 8 bits */
	while (i--) {
		/* LSB bit is first */
		OneWire_WriteBit(OneWireStruct, byte & 0x01);
		byte >>= 1;
	}
}
///////////////////////////////////////////////////////////////////////

// function to read 1-byte to the one-wire line
uint8_t OneWire_ReadByte(OneWire_t* OneWireStruct)
{
	uint8_t i = 8,byte;

	/* Read 8 bits */
	while (i--) {
		byte >>= 1;
		byte |= (OneWire_ReadBit(OneWireStruct) << 7);
	}	

	return byte;
}
///////////////////////////////////////////////////////////////////////

/********************************* END OF FILE ********************************/
/******************************************************************************/ 

