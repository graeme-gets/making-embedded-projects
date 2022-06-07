/*
 * mpu6050.c
 *
 *  Created on: Nov 13, 2019
 *      Author: Bulanov Konstantin
 *
 *  Contact information
 *  -------------------
 *
 * e-mail   :  leech001@gmail.com
 */

/*
 * |---------------------------------------------------------------------------------
 * | Copyright (C) Bulanov Konstantin,2019
 * |
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * |
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |
 * | Kalman filter algorithm used from https://github.com/TKJElectronics/KalmanFilter
 * |
 * | https://www.eluke.nl/2016/08/11/how-to-enable-motion-detection-interrupt-on-mpu6050/
 * |
 * | http://www.i2cdevlib.com/devices/mpu6050#registers
 * |---------------------------------------------------------------------------------
 */


#include <math.h>
#include "mpu6050.h"

#define RAD_TO_DEG 57.295779513082320876798154814105

#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define SMPLRT_DIV_REG 0x19
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_CONFIG_REG 0x1B
#define GYRO_XOUT_H_REG 0x43
#define SIG_PATH_RESET 0x68
#define INT_PIN_CFG 0x37
#define MOT_THR 0x1F
#define MOT_DUR 0x20
#define MOT_DETECT_CTRL 0x69
#define INT_ENABLE 0x38
#define MPU6050_REG_CONFIG 0x1A
#define MPU6050_REG_FIFO_EN       0x23


#define MPU6050_MOT_EN 1<< 6

// INT_PIN_CFG
#define MPU6050_INT_LEVEL 		1<<7
#define MPU6050_INT_OPEN		1<<6
#define MPU6050_LATCH_INT_EN	1<<5
#define MPU6050_INT_RD_CLEAR	1<<4
#define MPU6050_FSYNC_INT_LEVEL	1<<3
#define MPU6050_FSYNC_INT_EN	1<<2
#define MPU6050_I2C_BYPASS_EN	1<<1
#define MPU6050_CLKOUT_EN		1<<0

#define CAL_X 	-384
#define CAL_Y 	-1393
#define CAL_Z	-515346

// Setup MPU6050
#define MPU6050_ADDR 0xD0
const uint16_t i2c_timeout = 100;
const double Accel_Z_corrector = 14418.0;

uint32_t timer;

Kalman_t KalmanX = {
        .Q_angle = 0.001f,
        .Q_bias = 0.003f,
        .R_measure = 0.03f
};

Kalman_t KalmanY = {
        .Q_angle = 0.001f,
        .Q_bias = 0.003f,
        .R_measure = 0.03f,
};



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

}



uint8_t MPU6050_Init(I2C_HandleTypeDef *I2Cx)
{
    uint8_t Data;

    // check device ID WHO_AM_I
	Data = 0x4F;
	HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, i2c_timeout);

	// power management register 0X6B we should write all 0's to wake the sensor up
	Data = 1;
	HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, PWR_MGMT_1_REG, 1, &Data, 1, i2c_timeout);
	Data = 0;
	HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, PWR_MGMT_1_REG, 1, &Data, 1, i2c_timeout);

	Data = 0;
	HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, MPU6050_REG_CONFIG, 1, &Data, 1, i2c_timeout);

	Data = 0;
	HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, MPU6050_REG_FIFO_EN, 1, &Data, 1, i2c_timeout);

	// Reset Signal Path
	Data = 0x7; // Reset All
	HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, SIG_PATH_RESET, 1, &Data, 1, i2c_timeout);

	// Set the Interupt Pin
	Data = 0;// MPU6050_INT_LEVEL; // Active Low
	HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, INT_PIN_CFG, 1, &Data, 1, i2c_timeout);

	// Set accelerometer configuration in ACCEL_CONFIG Register
	// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> � 2g
	Data =0;
	HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, i2c_timeout);


	// Set Motion Threshold
	Data = 10;
	HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, MOT_THR, 1, &Data, 1, i2c_timeout);

	// Set Motion Duration
	Data = 1; //milli seconds
	HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, MOT_DUR, 1, &Data, 1, i2c_timeout);

	// Set Detection Decrement oand others
	Data = 20; // Decrement = 3
	HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, MOT_DETECT_CTRL, 1, &Data, 1, i2c_timeout);

	// Enable the Interrupt
	Data = 1<<6; //MPU6050_MOT_EN;
	HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, INT_ENABLE,1, &Data, 1, i2c_timeout);



    return 0;
}


void MPU6050_Read_Accel(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct) {
    uint8_t Rec_Data[6];

    // Read 6 BYTES of data starting from ACCEL_XOUT_H register

    HAL_I2C_Mem_Read(I2Cx, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 6, i2c_timeout);

    DataStruct->Accel_X_RAW = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->Accel_Y_RAW = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
    DataStruct->Accel_Z_RAW = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);

    /*** convert the RAW values into acceleration in 'g'
         we have to divide according to the Full scale value set in FS_SEL
         I have configured FS_SEL = 0. So I am dividing by 16384.0
         for more details check ACCEL_CONFIG Register              ****/

    // Callibrate
   // DataStruct->Accel_X_RAW += CAL_X;
   // DataStruct->Accel_Y_RAW += CAL_Y;
  //  DataStruct->Accel_Z_RAW += CAL_Y;


    DataStruct->Ax = DataStruct->Accel_X_RAW / 16384.0;
    DataStruct->Ay = DataStruct->Accel_Y_RAW / 16384.0;
    DataStruct->Az = DataStruct->Accel_Z_RAW / Accel_Z_corrector;


    if (DataStruct->Ax < 0.09) DataStruct->Ax = 0;
	if (DataStruct->Ay < 0.09) DataStruct->Ay= 0;
	if (DataStruct->Az < 0.09) DataStruct->Az = 0;


}


uint8_t  MPU6050_ReadReg(I2C_HandleTypeDef *I2Cx,uint16_t reg) // NOTE : Only for 1 Byte Reg at the moment
{
	uint8_t result;


	HAL_I2C_Mem_Read(I2Cx, MPU6050_ADDR, reg, 1, &result, 1, i2c_timeout);
	return result;
}

void MPU6050_WriteReg(I2C_HandleTypeDef *I2Cx,uint16_t reg, uint8_t data) // NOTE : Only for 1 Byte Reg at the moment
{
	HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, reg, 1, &data, 1, i2c_timeout);

}


void MPU6050_Read_Gyro(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct) {
    uint8_t Rec_Data[6];

    // Read 6 BYTES of data starting from GYRO_XOUT_H register

    HAL_I2C_Mem_Read(I2Cx, MPU6050_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, 6, i2c_timeout);

    DataStruct->Gyro_X_RAW = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->Gyro_Y_RAW = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
    DataStruct->Gyro_Z_RAW = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);

    /*** convert the RAW values into dps (�/s)
         we have to divide according to the Full scale value set in FS_SEL
         I have configured FS_SEL = 0. So I am dividing by 131.0
         for more details check GYRO_CONFIG Register              ****/

    DataStruct->Gx = DataStruct->Gyro_X_RAW / 131.0;
    DataStruct->Gy = DataStruct->Gyro_Y_RAW / 131.0;
    DataStruct->Gz = DataStruct->Gyro_Z_RAW / 131.0;
}

void MPU6050_Read_Temp(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct) {
    uint8_t Rec_Data[2];
    int16_t temp;

    // Read 2 BYTES of data starting from TEMP_OUT_H_REG register

    HAL_I2C_Mem_Read(I2Cx, MPU6050_ADDR, TEMP_OUT_H_REG, 1, Rec_Data, 2, i2c_timeout);

    temp = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->Temperature = (float) ((int16_t) temp / (float) 340.0 + (float) 36.53);
}


void MPU6050ReadStable(MPU6050_t *data)
{
	for (uint8_t cnt=0;cnt<30;cnt++)
		{
			MPU6050_Read_All(&I2C_MPU6050, data);
		}
}

void MPU6050_Read_All(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct) {
    uint8_t Rec_Data[14];
    int16_t temp;

    // Read 14 BYTES of data starting from ACCEL_XOUT_H register

    HAL_I2C_Mem_Read(I2Cx, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 14, i2c_timeout);

    DataStruct->Accel_X_RAW = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->Accel_Y_RAW = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
    DataStruct->Accel_Z_RAW = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);
    temp = (int16_t) (Rec_Data[6] << 8 | Rec_Data[7]);
    DataStruct->Gyro_X_RAW = (int16_t) (Rec_Data[8] << 8 | Rec_Data[9]);
    DataStruct->Gyro_Y_RAW = (int16_t) (Rec_Data[10] << 8 | Rec_Data[11]);
    DataStruct->Gyro_Z_RAW = (int16_t) (Rec_Data[12] << 8 | Rec_Data[13]);

    // Callibrate
	DataStruct->Accel_X_RAW += CAL_X;
	DataStruct->Accel_Y_RAW += CAL_Y;
	DataStruct->Accel_Z_RAW += CAL_Y;


    DataStruct->Ax = DataStruct->Accel_X_RAW / 16384.0;
    DataStruct->Ay = DataStruct->Accel_Y_RAW / 16384.0;
    DataStruct->Az = DataStruct->Accel_Z_RAW / Accel_Z_corrector;





    if (DataStruct->Ax < 0.09) DataStruct->Ax = 0;
    if (DataStruct->Ay < 0.09) DataStruct->Ay= 0;
    if (DataStruct->Az < 0.09) DataStruct->Az = 0;


    DataStruct->Temperature = (float) ((int16_t) temp / (float) 340.0 + (float) 36.53);
    DataStruct->Gx = DataStruct->Gyro_X_RAW / 131.0;
    DataStruct->Gy = DataStruct->Gyro_Y_RAW / 131.0;
    DataStruct->Gz = DataStruct->Gyro_Z_RAW / 131.0;

    // Kalman angle solve
    double dt = (double) (HAL_GetTick() - timer) / 1000;
    timer = HAL_GetTick();
    double roll;
    double roll_sqrt = sqrt(
            DataStruct->Accel_X_RAW * DataStruct->Accel_X_RAW + DataStruct->Accel_Z_RAW * DataStruct->Accel_Z_RAW);
    if (roll_sqrt != 0.0) {
        roll = atan(DataStruct->Accel_Y_RAW / roll_sqrt) * RAD_TO_DEG;
    } else {
        roll = 0.0;
    }
    double pitch = atan2(-DataStruct->Accel_X_RAW, DataStruct->Accel_Z_RAW) * RAD_TO_DEG;
    if ((pitch < -90 && DataStruct->KalmanAngleY > 90) || (pitch > 90 && DataStruct->KalmanAngleY < -90)) {
        KalmanY.angle = pitch;
        DataStruct->KalmanAngleY = pitch;
    } else {
        DataStruct->KalmanAngleY = Kalman_getAngle(&KalmanY, pitch, DataStruct->Gy, dt);
    }
    if (fabs(DataStruct->KalmanAngleY) > 90)
        DataStruct->Gx = -DataStruct->Gx;
    DataStruct->KalmanAngleX = Kalman_getAngle(&KalmanX, roll, DataStruct->Gy, dt);

}

double Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt) {
    double rate = newRate - Kalman->bias;
    Kalman->angle += dt * rate;

    Kalman->P[0][0] += dt * (dt * Kalman->P[1][1] - Kalman->P[0][1] - Kalman->P[1][0] + Kalman->Q_angle);
    Kalman->P[0][1] -= dt * Kalman->P[1][1];
    Kalman->P[1][0] -= dt * Kalman->P[1][1];
    Kalman->P[1][1] += Kalman->Q_bias * dt;

    double S = Kalman->P[0][0] + Kalman->R_measure;
    double K[2];
    K[0] = Kalman->P[0][0] / S;
    K[1] = Kalman->P[1][0] / S;

    double y = newAngle - Kalman->angle;
    Kalman->angle += K[0] * y;
    Kalman->bias += K[1] * y;

    double P00_temp = Kalman->P[0][0];
    double P01_temp = Kalman->P[0][1];

    Kalman->P[0][0] -= K[0] * P00_temp;
    Kalman->P[0][1] -= K[0] * P01_temp;
    Kalman->P[1][0] -= K[1] * P00_temp;
    Kalman->P[1][1] -= K[1] * P01_temp;

    return Kalman->angle;
};

