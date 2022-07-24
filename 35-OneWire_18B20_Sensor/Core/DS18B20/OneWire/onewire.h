/**
  ******************************************************************************
  * @file		onewire.h
  ******************************************************************************
  */
/********************/
/********///includes: 

	#include <stdint.h>
	#include "stm32f1xx_ll_gpio.h"
	#include "stm32f1xx_ll_tim.h"
/********************/
/********///definitions:

typedef struct {
	GPIO_TypeDef* GPIOx;           /*!< GPIOx port to be used for I/O functions */
	uint32_t GPIO_Pin;             /*!< GPIO Pin to be used for I/O functions */
	uint8_t LastDiscrepancy;       /*!< Search private */
	uint8_t LastFamilyDiscrepancy; /*!< Search private */
	uint8_t LastDeviceFlag;        /*!< Search private */
	uint8_t ROM_NO[8];             /*!< 8-bytes address of last search device */
} OneWire_t;

#define ONEWIRE_TIM	TIM1

/* OneWire commands */
#define ONEWIRE_CMD_RSCRATCHPAD			0xBE
#define ONEWIRE_CMD_WSCRATCHPAD			0x4E
#define ONEWIRE_CMD_CPYSCRATCHPAD		0x48
#define ONEWIRE_CMD_RECEEPROM				0xB8
#define ONEWIRE_CMD_RPWRSUPPLY			0xB4
#define ONEWIRE_CMD_SEARCHROM				0xF0
#define ONEWIRE_CMD_READROM					0x33
#define ONEWIRE_CMD_MATCHROM				0x55
#define ONEWIRE_CMD_SKIPROM					0xCC  	

/********************/
/********///Functions:  
  	
void OneWire_Delay(uint16_t time_us);

void OneWire_Low(OneWire_t *gp);	

void OneWire_High(OneWire_t *gp);

void OneWire_Input(OneWire_t *gp);
	
void OneWire_Output(OneWire_t *gp);

void OneWire_Init(OneWire_t* OneWireStruct, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);

uint8_t OneWire_Reset(OneWire_t* OneWireStruct);

void OneWire_WriteBit(OneWire_t* OneWireStruct, uint8_t bit);

uint8_t OneWire_ReadBit(OneWire_t* OneWireStruct); 

void OneWire_WriteByte(OneWire_t* OneWireStruct, uint8_t byte); 

uint8_t OneWire_ReadByte(OneWire_t* OneWireStruct);

/********************************* END OF FILE ********************************/
/******************************************************************************/ 

