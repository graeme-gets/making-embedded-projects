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




faceTable_t faceTable[12] = {
		{	1,	-12	,	8	,	-11	,	9	},
		{	2,	-11	,	9	,	61	,	81	},
		{	3,	58	,	78	,	30	,	50	},
		{	4,	23	,	43	,	-80	,	-60	},
		{	5,	-45	,	-25	,	-80	,	-60	},
		{	6,	-76	,	-56	,	33	,	53	},
		{	7,	-62	,	-42	,	-163	,	-143},
		{	8,	-40	,	-20	,	120	,	140	},
		{	9,	18	,	38	,	121	,	141	},
		{	10,	39	,	59	,	-162	,	-142	},
		{	11,	-12	,	8	,	-133	,	-113	},
		{	12,	-13	,	7	,	-188	,	-168}

};


int8_t detectFace(double x, double y)
{
	uint8_t face;
	for (face=0; face<FACE_COUNT; face++)
	{
		if ( x > faceTable[face].xRTop && x < faceTable[face].xRBottom && y > faceTable[face].yRTop && y < faceTable[face].yRBottom )
			return face;
	}
	return -1;
}



