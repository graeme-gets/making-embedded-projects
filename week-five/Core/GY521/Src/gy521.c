/*
 * gy521.c
 *
 *  Created on: Apr 27, 2022
 *      Author: GraemeGets
 */


// Private variables

#include "main.h"
#include "gy521.h"
#include "i2c.h"

	int16_t Gyro_X_RAW = 0;
	int16_t Gyro_Y_RAW = 0;
	int16_t Gyro_Z_RAW = 0;


uint8_t gy521_ready()
{
	uint8_t check;
	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR,WHO_AM_I_REG,1, &check, 1, 1000);
	return check == 114;
}

void gy521_Read_Gyro (gy521_t *gyro)
{
	uint8_t Rec_Data[6];

	// Read 6 BYTES of data starting from GYRO_XOUT_H register

	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, 6, 1000);

	Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	/*** convert the RAW values into dps (°/s)
		 we have to divide according to the Full scale value set in FS_SEL
		 I have configured FS_SEL = 0. So I am dividing by 131.0
		 for more details check GYRO_CONFIG Register              ****/

	gyro->Gx = Gyro_X_RAW/131.0;
	gyro->Gy = Gyro_Y_RAW/131.0;
	gyro->Gz = Gyro_Z_RAW/131.0;
}
