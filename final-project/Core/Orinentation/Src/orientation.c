/*
 * orientation.c
 *
 *  Created on: 30 May 2022
 *      Author: GraemeGets
 */


#include "mpu6050.h"
#include "systemConfig.h"
typedef struct
{
	uint8_t faceId;
	int16_t xRTop;
	int16_t xRBottom;
	int16_t yRTop;
	int16_t yRBottom;
} faceTable_t;

#define FACE_NOT_FOUND 0xFF


faceTable_t faceTable[12] = {
		{	0,	-12	,	8	,	-11	,	9		},
		{	1,	-11	,	9	,	61	,	81		},
		{	2,	58	,	78	,	30	,	50		},
		{	3,	23	,	43	,	-80	,	-60		},
		{	4,	-45	,	-25	,	-80	,	-60		},
		{	5,	-76	,	-56	,	33	,	53		},
		{	6,	-62	,	-42	,	-163,	-143	},
		{	7,	-40	,	-20	,	120	,	140		},
		{	8,	18	,	38	,	121	,	141		},
		{	9,	39	,	59	,	-162,	-142	},
		{	10,	-12	,	8	,	-133,	-113	},
		{	11,	-13	,	7	,	-188,	-168	}

};




int8_t detectFaceUp()
{
	MPU6050_t imuData;

	MPU6050ReadStable(&imuData);
	double x = imuData.KalmanAngleX;
	double y = imuData.KalmanAngleY;

	uint8_t face;
	for (face=0; face<FACE_COUNT; face++)
	{
		if ( x > faceTable[face].xRTop && x < faceTable[face].xRBottom && y > faceTable[face].yRTop && y < faceTable[face].yRBottom )
			return faceTable[face].faceId;
	}
	return FACE_NOT_FOUND;
}



