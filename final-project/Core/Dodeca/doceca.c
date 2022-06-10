/*
 * doceca.c
 *
 *  Created on: Jun 2, 2022
 *      Author: GraemeGets
 */
#include <dodeca.h>
#include <string.h>
#include <stdio.h>
#include "colours.h"
#include "ledController.h"
#include "dataStore.h"
#include "rtcController.h"
dodecaItems_t * dodecaItems = 0x0;


const dodecaItems_t defaultDoceca = {
		{
			{0,DODECA_DISABLED,"TOP",COLOUR_BLUE_ID, 0,0,0,DODECA_STATE_STOPPED},
			{1,DODECA_ENABLED,"Admin",COLOUR_GREEN_ID, 0,0,0,DODECA_STATE_STOPPED},
			{2,DODECA_ENABLED,"Coding",COLOUR_BRICK_ID, 0,0,0,DODECA_STATE_STOPPED},
			{3,DODECA_ENABLED,"Meeting",COLOUR_ORANGE_ID, 0,0,0,DODECA_STATE_STOPPED},
			{4,DODECA_ENABLED,"Client 1",COLOUR_PEACH_ID, 0,0,0,DODECA_STATE_STOPPED},
			{5,DODECA_ENABLED,"FaceBook",COLOUR_PURPLE_ID, 0,0,0,DODECA_STATE_STOPPED},
			{6,DODECA_DISABLED,"None",COLOUR_SKY_ID, 0,0,0,DODECA_STATE_STOPPED},
			{7,DODECA_DISABLED,"None",COLOUR_TEAL_ID, 0,0,0,DODECA_STATE_STOPPED},
			{8,DODECA_DISABLED,"None",COLOUR_YELLOW_ID, 0,0,0,DODECA_STATE_STOPPED},
			{9,DODECA_DISABLED,"None",COLOUR_SKY_ID, 0,0,0,DODECA_STATE_STOPPED},
			{10,DODECA_DISABLED,"None",COLOUR_WHITE_ID, 0,0,0,DODECA_STATE_STOPPED},
			{11,DODECA_DISABLED,"STOP",COLOUR_RED_ID, 0,0,0,DODECA_STATE_STOPPED},

		}
};


eDodecaErr_t dodecaInit(dodecaItems_t * items)
{
	dodecaItems = items;
	return DODECA_ERR_OK;
}

eDodecaErr_t dodecaReset()
{
	for (uint8_t i=0;i<DODECA_COUNT_MAX;i++)
	{
		dodecaItems->items[i].id = i;
		dodecaItems->items[i].colour = colourFindByid(defaultDoceca.items[i].colour)->code;
		dodecaItems->items[i].maxTimeMins = defaultDoceca.items[i].maxTimeMins;
		dodecaItems->items[i].minTimeMins = defaultDoceca.items[i].minTimeMins;
		dodecaItems->items[i].state = defaultDoceca.items[i].state;
		dodecaItems->items[i].enabled = defaultDoceca.items[i].enabled;
		strcpy(dodecaItems->items[i].name,defaultDoceca.items[i].name);
	}
	return DODECA_ERR_OK;
}

dodecaItem_t * dodecaGet(uint8_t id)
{
	if (id > DODECA_COUNT_MAX )
	{
		return NULL;
	}

	return   &dodecaItems->items[id];
}

eDodecaErr_t dodecaStart(uint8_t id)
{
	dodecaItem_t * dodeca ;
	dodeca = dodecaGet(id);
	dodeca->state = DODECA_STATE_ACTIVE;
	dodeca->startTime = rtcGetTimeStamp();
	ledSetFaceColour(id, dodeca->colour,0x0,LED_FACE_MODE_NORMAL );
	ledRender();
	return DODECA_ERR_OK;
}

eDodecaErr_t dodecaStop(uint8_t id)
{
	dodecaItem_t * dodeca ;
	dodeca = dodecaGet(id);
	dodeca->state = DODECA_STATE_STOPPED;
	ledSetFaceColour(id, colourFindByid(COLOUR_BLACK_ID)->code,0x0,LED_FACE_MODE_NORMAL );
	ledRender();

	// Create a Dodeca record st store
	recordDodeca_t record;
	record.dodecaId = id;
	record.startTime = dodeca->startTime;
	record.endTime = rtcGetTimeStamp();
	if (DATA_STORAGE_ERR_OK != dataStoreAdd(record))
	{
		return DODECA_ERR_ERROR;
	}
	return DODECA_ERR_OK;
}

eDodecaErr_t dodecaGetByState(const eDodecaState_t state, dodecaItem_t *dodeca)
{
	for (uint8_t i=0;i<DODECA_COUNT_MAX;i++)
	{
		if (state == dodecaItems->items[i].state)
		{
			dodeca = &dodecaItems->items[i];
			return DODECA_ERR_OK;
		}
	}
	return DODECA_ERR_NOT_FOUND;
}



eDodecaErr_t dodecaGetStateName(eDodecaState_t state,char* name)
{

	switch (state)
	{
		case DODECA_STATE_STOPPED : strcpy(name,"Stopped"); break;
		case DODECA_STATE_ACTIVE : strcpy(name,"Active"); break;
		default: strcpy(name,"undefined"); break;
	}

	return DODECA_ERR_OK;


}
