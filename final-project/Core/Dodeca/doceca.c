/*
 * doceca.c
 *
 *  Created on: Jun 2, 2022
 *      Author: GraemeGets
 */
#include <dodeca.h>
#include <string.h>

dodecaItems_t * dodecaItems = 0x0;

eDodecaErr_t dodecaInit(dodecaItems_t * items)
{
	dodecaItems = items;
	return DODECA_ERR_OK;
}

dodecaItem_t *dodecaGet(uint8_t id)
{
	if (id > DODECA_COUNT_MAX )
	{
		return 0x0;
	}

	return &dodecaItems->items[id];
}

eDodecaErr_t dodecaInitItem(uint8_t id, uint16_t min, uint16_t max, uint8_t taskId)
{
	dodecaItem_t *dodeca;
	dodeca = dodecaGet(id);
	if (!dodeca) return DODECA_ERR_INVALID_ID;

	dodeca->state = DODECA_STATE_STOPPED;
	dodeca->maxTimeMins = max;
	dodeca->minTimeMins = min;
	dodeca->taskId = taskId;
	return DODECA_ERR_OK;
}


eDodecaErr_t dodecaGetStateName(eDodecaState_t state,char* name)
{

	switch (state)
	{
		case DODECA_STATE_NOT_CONFIGURED : strcpy(name,"Not Configured"); break;
		case DODECA_STATE_STOPPED : strcpy(name,"Stopped"); break;
		case DODECA_STATE_ACTIVE : strcpy(name,"Active"); break;
		default: strcpy(name,"undefined"); break;
	}

	return DODECA_ERR_OK;


}