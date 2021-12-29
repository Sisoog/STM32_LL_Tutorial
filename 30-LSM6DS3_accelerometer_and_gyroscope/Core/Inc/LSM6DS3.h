/**
  ******************************************************************************
  * @file		LSM6DS3.h
  ******************************************************************************
  */  
  
	#include "stdio.h"
	#include "stdint.h"
	#include "stm32f1xx_ll_spi.h"
	#include "stm32f1xx_ll_utils.h"
	#include "stm32f1xx_ll_gpio.h"
	
/************** Device Register  *******************/
#define LSM6DS3_ACC_GYRO_TEST_PAGE  			0X00
#define LSM6DS3_ACC_GYRO_RAM_ACCESS  			0X01
#define LSM6DS3_ACC_GYRO_SENSOR_SYNC_TIME  		0X04
#define LSM6DS3_ACC_GYRO_SENSOR_SYNC_EN  		0X05
#define LSM6DS3_ACC_GYRO_FIFO_CTRL1  			0X06
#define LSM6DS3_ACC_GYRO_FIFO_CTRL2  			0X07
#define LSM6DS3_ACC_GYRO_FIFO_CTRL3  			0X08
#define LSM6DS3_ACC_GYRO_FIFO_CTRL4  			0X09
#define LSM6DS3_ACC_GYRO_FIFO_CTRL5  			0X0A
#define LSM6DS3_ACC_GYRO_ORIENT_CFG_G  			0X0B
#define LSM6DS3_ACC_GYRO_REFERENCE_G  			0X0C
#define LSM6DS3_ACC_GYRO_INT1_CTRL  			0X0D
#define LSM6DS3_ACC_GYRO_INT2_CTRL  			0X0E
#define LSM6DS3_ACC_GYRO_WHO_AM_I_REG  			0X0F
#define LSM6DS3_ACC_GYRO_CTRL1_XL  			0X10
#define LSM6DS3_ACC_GYRO_CTRL2_G  			0X11
#define LSM6DS3_ACC_GYRO_CTRL3_C  			0X12
#define LSM6DS3_ACC_GYRO_CTRL4_C  			0X13
#define LSM6DS3_ACC_GYRO_CTRL5_C  			0X14
#define LSM6DS3_ACC_GYRO_CTRL6_G  			0X15
#define LSM6DS3_ACC_GYRO_CTRL7_G  			0X16
#define LSM6DS3_ACC_GYRO_CTRL8_XL  			0X17
#define LSM6DS3_ACC_GYRO_CTRL9_XL  			0X18
#define LSM6DS3_ACC_GYRO_CTRL10_C  			0X19
#define LSM6DS3_ACC_GYRO_MASTER_CONFIG  		0X1A
#define LSM6DS3_ACC_GYRO_WAKE_UP_SRC  			0X1B
#define LSM6DS3_ACC_GYRO_TAP_SRC  			0X1C
#define LSM6DS3_ACC_GYRO_D6D_SRC  			0X1D
#define LSM6DS3_ACC_GYRO_STATUS_REG  			0X1E
#define LSM6DS3_ACC_GYRO_OUT_TEMP_L  			0X20
#define LSM6DS3_ACC_GYRO_OUT_TEMP_H  			0X21
#define LSM6DS3_ACC_GYRO_OUTX_L_G  			0X22
#define LSM6DS3_ACC_GYRO_OUTX_H_G  			0X23
#define LSM6DS3_ACC_GYRO_OUTY_L_G  			0X24
#define LSM6DS3_ACC_GYRO_OUTY_H_G  			0X25
#define LSM6DS3_ACC_GYRO_OUTZ_L_G  			0X26
#define LSM6DS3_ACC_GYRO_OUTZ_H_G  			0X27
#define LSM6DS3_ACC_GYRO_OUTX_L_XL  			0X28
#define LSM6DS3_ACC_GYRO_OUTX_H_XL  			0X29
#define LSM6DS3_ACC_GYRO_OUTY_L_XL  			0X2A
#define LSM6DS3_ACC_GYRO_OUTY_H_XL  			0X2B
#define LSM6DS3_ACC_GYRO_OUTZ_L_XL  			0X2C
#define LSM6DS3_ACC_GYRO_OUTZ_H_XL  			0X2D
#define LSM6DS3_ACC_GYRO_SENSORHUB1_REG  		0X2E
#define LSM6DS3_ACC_GYRO_SENSORHUB2_REG  		0X2F
#define LSM6DS3_ACC_GYRO_SENSORHUB3_REG  		0X30
#define LSM6DS3_ACC_GYRO_SENSORHUB4_REG  		0X31
#define LSM6DS3_ACC_GYRO_SENSORHUB5_REG  		0X32
#define LSM6DS3_ACC_GYRO_SENSORHUB6_REG  		0X33
#define LSM6DS3_ACC_GYRO_SENSORHUB7_REG  		0X34
#define LSM6DS3_ACC_GYRO_SENSORHUB8_REG  		0X35
#define LSM6DS3_ACC_GYRO_SENSORHUB9_REG  		0X36
#define LSM6DS3_ACC_GYRO_SENSORHUB10_REG  		0X37
#define LSM6DS3_ACC_GYRO_SENSORHUB11_REG  		0X38
#define LSM6DS3_ACC_GYRO_SENSORHUB12_REG  		0X39
#define LSM6DS3_ACC_GYRO_FIFO_STATUS1  			0X3A
#define LSM6DS3_ACC_GYRO_FIFO_STATUS2  			0X3B
#define LSM6DS3_ACC_GYRO_FIFO_STATUS3  			0X3C
#define LSM6DS3_ACC_GYRO_FIFO_STATUS4  			0X3D
#define LSM6DS3_ACC_GYRO_FIFO_DATA_OUT_L  		0X3E
#define LSM6DS3_ACC_GYRO_FIFO_DATA_OUT_H  		0X3F
#define LSM6DS3_ACC_GYRO_TIMESTAMP0_REG  		0X40
#define LSM6DS3_ACC_GYRO_TIMESTAMP1_REG  		0X41
#define LSM6DS3_ACC_GYRO_TIMESTAMP2_REG  		0X42
#define LSM6DS3_ACC_GYRO_STEP_COUNTER_L  		0X4B
#define LSM6DS3_ACC_GYRO_STEP_COUNTER_H  		0X4C
#define LSM6DS3_ACC_GYRO_FUNC_SRC  			0X53
#define LSM6DS3_ACC_GYRO_TAP_CFG1  			0X58
#define LSM6DS3_ACC_GYRO_TAP_THS_6D  			0X59
#define LSM6DS3_ACC_GYRO_INT_DUR2  			0X5A
#define LSM6DS3_ACC_GYRO_WAKE_UP_THS  			0X5B
#define LSM6DS3_ACC_GYRO_WAKE_UP_DUR  			0X5C
#define LSM6DS3_ACC_GYRO_FREE_FALL  			0X5D
#define LSM6DS3_ACC_GYRO_MD1_CFG  			0X5E
#define LSM6DS3_ACC_GYRO_MD2_CFG  			0X5F

/************** Access Device RAM  *******************/
#define LSM6DS3_ACC_GYRO_ADDR0_TO_RW_RAM         0x62
#define LSM6DS3_ACC_GYRO_ADDR1_TO_RW_RAM         0x63
#define LSM6DS3_ACC_GYRO_DATA_TO_WR_RAM          0x64
#define LSM6DS3_ACC_GYRO_DATA_RD_FROM_RAM        0x65

#define LSM6DS3_ACC_GYRO_RAM_SIZE                4096

/************** Embedded functions register mapping  *******************/
#define LSM6DS3_ACC_GYRO_SLV0_ADD                     0x02
#define LSM6DS3_ACC_GYRO_SLV0_SUBADD                  0x03
#define LSM6DS3_ACC_GYRO_SLAVE0_CONFIG                0x04
#define LSM6DS3_ACC_GYRO_SLV1_ADD                     0x05
#define LSM6DS3_ACC_GYRO_SLV1_SUBADD                  0x06
#define LSM6DS3_ACC_GYRO_SLAVE1_CONFIG                0x07
#define LSM6DS3_ACC_GYRO_SLV2_ADD                     0x08
#define LSM6DS3_ACC_GYRO_SLV2_SUBADD                  0x09
#define LSM6DS3_ACC_GYRO_SLAVE2_CONFIG                0x0A
#define LSM6DS3_ACC_GYRO_SLV3_ADD                     0x0B
#define LSM6DS3_ACC_GYRO_SLV3_SUBADD                  0x0C
#define LSM6DS3_ACC_GYRO_SLAVE3_CONFIG                0x0D
#define LSM6DS3_ACC_GYRO_DATAWRITE_SRC_MODE_SUB_SLV0  0x0E
#define LSM6DS3_ACC_GYRO_CONFIG_PEDO_THS_MIN          0x0F
#define LSM6DS3_ACC_GYRO_CONFIG_TILT_IIR              0x10
#define LSM6DS3_ACC_GYRO_CONFIG_TILT_ACOS             0x11
#define LSM6DS3_ACC_GYRO_CONFIG_TILT_WTIME            0x12
#define LSM6DS3_ACC_GYRO_SM_STEP_THS                  0x13
#define LSM6DS3_ACC_GYRO_MAG_SI_XX                    0x24
#define LSM6DS3_ACC_GYRO_MAG_SI_XY                    0x25
#define LSM6DS3_ACC_GYRO_MAG_SI_XZ                    0x26
#define LSM6DS3_ACC_GYRO_MAG_SI_YX                    0x27
#define LSM6DS3_ACC_GYRO_MAG_SI_YY                    0x28
#define LSM6DS3_ACC_GYRO_MAG_SI_YZ                    0x29
#define LSM6DS3_ACC_GYRO_MAG_SI_ZX                    0x2A
#define LSM6DS3_ACC_GYRO_MAG_SI_ZY                    0x2B
#define LSM6DS3_ACC_GYRO_MAG_SI_ZZ                    0x2C
#define LSM6DS3_ACC_GYRO_MAG_OFFX_L                   0x2D
#define LSM6DS3_ACC_GYRO_MAG_OFFX_H                   0x2E
#define LSM6DS3_ACC_GYRO_MAG_OFFY_L                   0x2F
#define LSM6DS3_ACC_GYRO_MAG_OFFY_H                   0x30
#define LSM6DS3_ACC_GYRO_MAG_OFFZ_L                   0x31
#define LSM6DS3_ACC_GYRO_MAG_OFFZ_H                   0x32


/********************/
/********///Functions:

uint8_t converse_SPI(uint8_t);
int16_t LSM6DS3_read_reg (uint8_t, uint8_t);
void	LSM6DS3_write_reg (uint8_t, uint8_t);
uint8_t get_accelRange(void);
float LSM6DS3_calcAccel(int16_t);
float LSM6DS3_readFloatAccelX( void );
float LSM6DS3_readFloatAccelY( void );
float LSM6DS3_readFloatAccelZ( void );
uint16_t get_gyroRange(void);
float LSM6DS3_calcGyro(int16_t);
float LSM6DS3_readFloatGyroX(void);
float LSM6DS3_readFloatGyroY(void);
float LSM6DS3_readFloatGyroZ(void);
void initialize_LSM6DS3 (void);
void print_gyro_uncal(void);
void print_acc_uncal(void);
void print_gyro_cal(void);
void print_acc_cal(void);
/********************************* END OF FILE ********************************/
/******************************************************************************/
