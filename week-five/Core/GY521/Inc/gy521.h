/*
 * gy521.h
 *
 *  Created on: Apr 27, 2022
 *      Author: GraemeGets
 */

#ifndef GY521_INC_GY521_H_
#define GY521_INC_GY521_H_

typedef struct {
	float Ax, Ay, Az, Gx, Gy, Gz;
}gy521_t;


#define MPU6050_ADDR 0xD0

#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define TEMP_OUT_L_REG 0x42
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75


void gy521_Read_Gyro (gy521_t *gyro);
uint8_t gy521_ready();

#endif /* GY521_INC_GY521_H_ */
