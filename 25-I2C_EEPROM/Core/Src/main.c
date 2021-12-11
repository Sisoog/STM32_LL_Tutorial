/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define X_NOP()		{__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();}
#define Y_NOP()		X_NOP();X_NOP();X_NOP();X_NOP();X_NOP();X_NOP();X_NOP();X_NOP();X_NOP();X_NOP();
#define Z_NOP()		Y_NOP();Y_NOP();Y_NOP();Y_NOP();Y_NOP();Y_NOP();Y_NOP();Y_NOP();Y_NOP();Y_NOP();
#define Z2_NOP()	Z_NOP();Z_NOP();Z_NOP();Z_NOP();Z_NOP();Z_NOP();Z_NOP();Z_NOP();Z_NOP();Z_NOP();

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

	/* Function for transmitting 8bit data via I2C */
void write_i2c(uint8_t Data)
{
	LL_I2C_TransmitData8(I2C1, Data); 
	while(!LL_I2C_IsActiveFlag_TXE(I2C1));
}

	/* Function for receiving 8bit data via I2C */
uint8_t read_i2c(bool IsAck)
{
	if(!IsAck)
		LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
	else
		LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
	
	uint8_t Data;
	while(!LL_I2C_IsActiveFlag_RXNE(I2C1));
	Data = LL_I2C_ReceiveData8(I2C1);
	return Data;
}

	/* Function for transmitting 8bit data via USART */
	void write_uart(char data)		
	{
		while(!LL_USART_IsActiveFlag_TXE(USART1));
		LL_USART_TransmitData8(USART1, (uint8_t)data);
	}
	
	/* Retargeting stdout_putchar as to use USART_TX for data output */	
	int stdout_putchar (int ch) {
	  write_uart(ch);
	  return (ch);
	}
	
	
//////////////////////////////////////
#define I2C_REQUEST_WRITE                       0x00
#define I2C_REQUEST_READ                        0x01
#define I2C_Slave_Adr														(0xA0)
	
		/* Function for writing 8bit data to the specified address in EEPROM via I2C */
bool E2Prom_Write(uint16_t adr,uint8_t data)
{
	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
	
	LL_I2C_GenerateStartCondition(I2C1);
	Z_NOP();
	
	LL_I2C_TransmitData8(I2C1, I2C_Slave_Adr | I2C_REQUEST_WRITE);    	// Set Address of the slave, Enable Write mode
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1)) {}; 													// Loop until ADDR flag is raised
  LL_I2C_ClearFlag_ADDR(I2C1);																				// Clear ADDR flag value in ISR register
		
	/// memory address
	write_i2c(adr>>8);
	write_i2c(adr&0xFF);
	
	write_i2c(data);
	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);	
	while(!LL_I2C_IsActiveFlag_TXE(I2C1));
	LL_I2C_GenerateStopCondition(I2C1);
		
	return true;
}

		/* Function for reading 8bit data from the specified address in EEPROM via I2C */
uint8_t E2Prom_Read(uint16_t adr)
{
	uint8_t data;
	LL_I2C_GenerateStartCondition(I2C1);
	Z_NOP();
	
	LL_I2C_TransmitData8(I2C1, I2C_Slave_Adr | I2C_REQUEST_WRITE);    	// Set Address of the slave, Enable Write mode
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1)) {}; 													// Loop until ADDR flag is raised
  LL_I2C_ClearFlag_ADDR(I2C1);																				// Clear ADDR flag value in ISR register
		
	/// memory address
	write_i2c(adr>>8);
	write_i2c(adr&0xFF);
			

	LL_I2C_GenerateStartCondition(I2C1);
	Z2_NOP();

	LL_I2C_TransmitData8(I2C1, I2C_Slave_Adr | I2C_REQUEST_READ);    	// Set Address of the slave, Enable Write mode
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1)) {}; 	// Loop until ADDR flag is raised
  LL_I2C_ClearFlag_ADDR(I2C1);								// Clear ADDR flag value in ISR register

		
	data = read_i2c(false);	
	
	LL_I2C_GenerateStopCondition(I2C1);
		
	return data;	
}
////////////////////////////////////
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
  */
  LL_GPIO_AF_Remap_SWJ_NOJTAG();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

	uint8_t buffer1, buffer2;
	LL_I2C_Enable(I2C1);
	
	

		E2Prom_Write(0x0000,0x24);								// write 0x24 to the address 0000 of the EEPROM
		LL_mDelay(5);															// Duration of the EEPROM internal write cycle
		E2Prom_Write(0x0001,0x25);
		LL_mDelay(5);
		buffer1 = E2Prom_Read(0x0000);						// read from the address 0000 of the EEPROM
		LL_mDelay(1);															// Delay needed for each read cycle
		buffer2 = E2Prom_Read(0x0001);
		LL_mDelay(1);
		
		printf("buffer1 is: %x\r\n", buffer1);	
		printf("buffer2 is: %x\r\n", buffer2);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(72000000);
  LL_SetSystemCoreClock(72000000);
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB6   ------> I2C1_SCL
  PB7   ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  /** I2C Initialization
  */
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.ClockSpeed = 100000;
  I2C_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_2;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C1, 0);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
