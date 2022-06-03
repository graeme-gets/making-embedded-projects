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
	DODECA_STATE_NOT_CONFIGURED = 1,
	DODECA_STATE_ACTIVE = 2,
	DODECA_STATE_STOPPED = 3,
	DODECA_STATE_32Bit = 0xFFFFFF
}eDodecaState_t;


typedef enum
{
	DODECA_ERR_OK = 0u,
	DODECA_ERR_INVALID_ID = 1u
}eDodecaErr_t;

typedef struct
{
	uint8_t taskId;
	uint8_t minTimeMins;
	uint8_t maxTimeMins;
	eDodecaState_t state;
} dodecaItem_t;

typedef struct
{
	dodecaItem_t items[DODECA_COUNT_MAX];
}dodecaItems_t;


eDodecaErr_t dodecaInit(dodecaItems_t * items);
dodecaItem_t * dodecaGet(uint8_t id);
eDodecaErr_t dodecaGetStateName(eDodecaState_t state,char* name);
eDodecaErr_t dodecaInitItem(uint8_t id, uint16_t min, uint16_t max, uint8_t taskId);

#endif /* DODECA_INC_DODECA_H_ */
