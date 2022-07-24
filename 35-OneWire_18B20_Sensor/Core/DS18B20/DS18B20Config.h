/**
  ******************************************************************************
  * @file		DS18B20Config.h
  ******************************************************************************
  */
  
 //	config one timer to have a 1us clock	(for cpu clock = 72 MHz  >>> Prescaler=(72-1) , counter period=Max)
 // for testing this code TIMER1 is configured and defined as ONEWIRE_TIM in onewire.h file
//**************************************************************************************************************//
  

/********************/
/********///definitions:

///////////Config:
#define	_DS18B20_GPIO						GPIOA								// port used for one wire connection
#define	_DS18B20_PIN						LL_GPIO_PIN_8				// pin used for one wire connection(signal pin of the 18B20 sensor)

#define DS18B20_Scaling_factor	0.0078125						// Scaling factor for the sensor's converted temperature

/********************************* END OF FILE ********************************/
/******************************************************************************/ 

