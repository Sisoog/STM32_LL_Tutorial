/**
  ******************************************************************************
  * @file		LSM6DS3.c
  ******************************************************************************
  */  
  
	#include "LSM6DS3.h"


	#define LSM_Port GPIOC;
	#define CS_Pin LL_GPIO_PIN_0
	
	uint16_t Acc_x, Acc_y, Acc_z, Gyro_x, Gyro_y, Gyro_z;
	uint8_t Read = 0x80;
	uint8_t Write = 0x00;

/********************/
/********///Functions:

//****************************************************************************//
//
//  Basic
//
//****************************************************************************//

//	uint8_t converse_SPI(uint8_t wdata)
//	{
//			while(!LL_SPI_IsActiveFlag_TXE(SPI1));
//			LL_SPI_TransmitData8(SPI1, wdata);
//			while(!LL_SPI_IsActiveFlag_RXNE(SPI1));
//			return LL_SPI_ReceiveData8(SPI1);	
//	}

	/* function to transmit or receive one byte through SPI1 unit */
	uint8_t converse_SPI(uint8_t wdata)
	{
	while (!(SPI1->SR & SPI_SR_TXE))
		{};
  // Send byte over the SPI
  LL_SPI_TransmitData8(SPI1,wdata);
  // Wait until the transmission is complete
  while (SPI1->SR & SPI_SR_BSY);
	// receive byte
	while(!LL_SPI_IsActiveFlag_RXNE(SPI1));
	return LL_SPI_ReceiveData8(SPI1);	
	}		
	
	/* function to transmit or receive two bytes through SPI1 unit */
		uint16_t converse_SPI16(uint16_t wdata)
	{
	while (!(SPI1->SR & SPI_SR_TXE))
		{};
  // Send byte over the SPI
  LL_SPI_TransmitData16(SPI1,wdata);
  // Wait until the transmission is complete
  while (SPI1->SR & SPI_SR_BSY);
	// receive byte
	while(!LL_SPI_IsActiveFlag_RXNE(SPI1));
	return LL_SPI_ReceiveData16(SPI1);	
	}		
	
	/* function to write one byte(data) into the register identified by the Address addr in LSM6DS3 module */
	void LSM6DS3_write_reg (uint8_t addr, uint8_t data)
	{
		LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0);
		converse_SPI(Write | addr);
		converse_SPI(data);
		LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_0);
	}

	/* function to read one or two bytes(data) from the register identified by the Address addr in LSM6DS3 module */	
	int16_t LSM6DS3_read_reg (uint8_t addr, uint8_t rcount)
	{
	  int16_t data;	
		LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0);
		converse_SPI(Read | addr);

		
		data = converse_SPI(0xFF);
		
		if(rcount == 1)
		{
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_0);
			return data;
		}
		else if(rcount == 2)
		{
			data |= converse_SPI(0xFF) << 8;
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_0);
			return data;	
		}
		else
			return 0;
	}

//	int16_t LSM6DS3_read_reg (uint8_t addr, uint8_t rcount)
//	{
//		int16_t data;
//		LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0);
//		converse_SPI(Read | addr);
//		

//		
//		if(rcount == 1)
//		{
//			data = converse_SPI(0xFF);
//			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_0);
//			return data;
//		}
//		else if(rcount == 2)
//		{
//			data = converse_SPI16(0xFFFF);
//			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_0);
//			return data;	
//		}
//		else
//			return 0;
//	}
//****************************************************************************//
//
//  Accelerometer
//
//****************************************************************************//
	/* function to get the accelerometer range setting */	
	uint8_t get_accelRange(void)
	{
		uint8_t range = LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_CTRL1_XL, 1) & 0x0C;
		range = range >> 2;
		switch(range)
		{
			case 0x0: return 1;

		  case 0x1: return 8;

		  case 0x2: return 2;

		  case 0x3: return 4;
			
			default : return 0;
		}
	}

	/* function to calculated calibrated values for the accelerometer  */	
	float LSM6DS3_calcAccel(int16_t input)
	{
		float output = (float)input * 0.061 *  get_accelRange() / 1000;
		return output;
	}

	/* function to read X value of the accelerometer  */	
	float LSM6DS3_readFloatAccelX(void)
	{
		float output = LSM6DS3_calcAccel(LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_OUTX_L_XL, 2));
		return output;
	}
	
	/* function to read Y value of the accelerometer  */
	float LSM6DS3_readFloatAccelY(void)
	{
		float output = LSM6DS3_calcAccel(LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_OUTY_L_XL, 2));
		return output;
	}

	/* function to read Z value of the accelerometer  */	
	float LSM6DS3_readFloatAccelZ(void)
	{
		float output = LSM6DS3_calcAccel(LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_OUTZ_L_XL, 2));
		return output;
	}

//****************************************************************************//
//
//  Gyroscope
//
//****************************************************************************//
	/* function to get the gyroscope range setting */
	uint16_t get_gyroRange(void)
	{
		uint8_t range = LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_CTRL2_G, 1) & 0x0C;
		range = range >> 2;
		switch(range)
		{
			case 0x0: return 245;

		  case 0x1: return 500;

		  case 0x2: return 1000;

		  case 0x3: return 2000;
			
			default : return 0;
		}
	}
	
	/* function to calculated calibrated values for the gyroscope  */	
	float LSM6DS3_calcGyro(int16_t input)
{
	uint8_t gyroRangeDivisor = /* get_gyroRange() */ 245 / 125;
	if ( get_gyroRange() == 245 )
		gyroRangeDivisor = 2;


	float output = (float)input * 4.375 * (gyroRangeDivisor) / 1000;
	return output;
}

	/* function to read X value of the gyroscope  */
	float LSM6DS3_readFloatGyroX(void)
	{
		float output = LSM6DS3_calcGyro(LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_OUTX_L_G, 2));
		return output;
	}

	/* function to read Y value of the gyroscope  */
	float LSM6DS3_readFloatGyroY(void)
	{
		float output = LSM6DS3_calcGyro(LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_OUTY_L_G, 2));
		return output;
	}

	/* function to read Z value of the gyroscope  */	
	float LSM6DS3_readFloatGyroZ(void)
	{
		float output = LSM6DS3_calcGyro(LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_OUTZ_L_G, 2));
		return output;
	}

//****************************************************************************//
//
//  utility functions
//
//****************************************************************************//
	/* initializing the LSM6DS3 module */
	void initialize_LSM6DS3 (void)
	{
		/* default values for LSM6DS3 config registers*/
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_RAM_ACCESS, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_SENSOR_SYNC_TIME, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_FIFO_CTRL1, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_FIFO_CTRL2, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_FIFO_CTRL3, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_FIFO_CTRL4, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_FIFO_CTRL5, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_ORIENT_CFG_G, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_CTRL2_G, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_CTRL4_C, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_CTRL5_C, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_CTRL6_G, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_CTRL7_G, 0x00);		
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_CTRL8_XL, 0x00);
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_CTRL9_XL, 0x38);			
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_CTRL10_C, 0x38);
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_MASTER_CONFIG, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_WAKE_UP_SRC, 0x00);
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_TAP_SRC, 0x00); 
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_D6D_SRC, 0x00);
		

		/* Enable & Configure Accelerometer */
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_CTRL1_XL, 0xA0);   	
		/* Enable & Configure Gyroscope */			
		LSM6DS3_write_reg(LSM6DS3_ACC_GYRO_CTRL2_G, 0x80);
		
		printf("Initialization done!\tWHO_AM_I Register: %x\r\n", LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_WHO_AM_I_REG, 1));
	}
	
		/* print the uncalibrated x,y and z values of the gyroscope  */
	void print_gyro_uncal(void)
	{
			printf("Uncalibrated Gyroscope parameters:\r\n");
			printf("  X: %d\r\n", LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_OUTX_L_G, 2));
			printf("  Y: %d\r\n", LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_OUTY_L_G, 2));		
			printf("  Z: %d\r\n", LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_OUTZ_L_G, 2)); 
			printf("\r\n");
	}

		/* print the uncalibrated x,y and z values of the accelerometer  */	
	void print_acc_uncal(void)
	{
			printf("Uncalibrated Accelerometer parameters:\r\n");
			printf("  X: %d\r\n", LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_OUTX_L_XL, 2));
			printf("  Y: %d\r\n", LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_OUTY_L_XL, 2));		
			printf("  Z: %d\r\n", LSM6DS3_read_reg(LSM6DS3_ACC_GYRO_OUTZ_L_XL, 2)); 
			printf("\r\n");
	}

		/* print the calibrated x,y and z values of the gyroscope  */	
	void print_gyro_cal(void)
	{
			printf("Calibrated Gyroscope parameters:\r\n");
			printf("\r\n\t X:\t %3.3f DPS", LSM6DS3_readFloatGyroX());
			printf("\r\n\t Y:\t %3.3f DPS", LSM6DS3_readFloatGyroY());
			printf("\r\n\t Z:\t %3.3f DPS", LSM6DS3_readFloatGyroZ());
			printf("\r\n");
	}

		/* print the calibrated x,y and z values of the accelerometer  */		
	void print_acc_cal(void)
	{
			printf("Calibrated Accelerometer parameters:\r\n");
			printf("\r\n\t X:\t %3.3f g", LSM6DS3_readFloatAccelX());
			printf("\r\n\t Y:\t %3.3f g", LSM6DS3_readFloatAccelY());
			printf("\r\n\t Z:\t %3.3f g", LSM6DS3_readFloatAccelZ());
			printf("\r\n");
	}	
/********************************* END OF FILE ********************************/
/******************************************************************************/	
