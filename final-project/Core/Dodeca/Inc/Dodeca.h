/*
 * dodeca.h
 *
 *  Created on: Jun 2, 2022
 *      Author: GraemeGets
 */

#ifndef DODECA_INC_DODECA_H_
#define DODECA_INC_DODECA_H_

#include "Tasks.h"

#define DODECA_COUNT_MAX 12

typedef enum
{
	DODECA_NOT_CONFIGURED = 1,
	DODECA_TIMING = 2,
	DODECA_STOPPED = 3,
	DODECA_32Bit = 0xFFFFFF
}eDodecaState_t;


typedef struct
{
	uint8_t id;
	uint8_t taskId;
	uint8_t MinTimeMins;
	uint8_t MaxTimeMins;
	eDodecaState_t state;
} dodecaItem_t;

typedef struct
{
	dodecaItem_t items[DODECA_COUNT_MAX];
}dodecaItems_t;

#endif /* DODECA_INC_DODECA_H_ */
