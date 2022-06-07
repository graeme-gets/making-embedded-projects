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
dodecaItems_t * dodecaItems = 0x0;


const dodecaItems_t defaultDoceca = {
		{
			{"FaceBook",COLOUR_BLUE_ID, 0,0,DODECA_STATE_STOPPED},
			{"Admin",COLOUR_GREEN_ID, 0,0,DODECA_STATE_STOPPED},
			{"Coding",COLOUR_BRICK_ID, 0,0,DODECA_STATE_STOPPED},
			{"Meeting",COLOUR_ORANGE_ID, 0,0,DODECA_STATE_STOPPED},
			{"None",COLOUR_PEACH_ID, 0,0,DODECA_STATE_NOT_CONFIGURED},
			{"None",COLOUR_PURPLE_ID, 0,0,DODECA_STATE_NOT_CONFIGURED},
			{"None",COLOUR_SKY_ID, 0,0,DODECA_STATE_NOT_CONFIGURED},
			{"None",COLOUR_TEAL_ID, 0,0,DODECA_STATE_NOT_CONFIGURED},
			{"None",COLOUR_YELLOW_ID, 0,0,DODECA_STATE_NOT_CONFIGURED},
			{"None",COLOUR_SKY_ID, 0,0,DODECA_STATE_NOT_CONFIGURED},
			{"None",COLOUR_WHITE_ID, 0,0,DODECA_STATE_NOT_CONFIGURED},
			{"STOP",COLOUR_RED_ID, 0,0,DODECA_STATE_STOPPED},

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
		dodecaItems->items[i].colour = colourFindByid(defaultDoceca.items[i].colour)->code;
		dodecaItems->items[i].maxTimeMins = defaultDoceca.items[i].maxTimeMins;
		dodecaItems->items[i].minTimeMins = defaultDoceca.items[i].minTimeMins;
		dodecaItems->items[i].state = defaultDoceca.items[i].state;
		strcpy(dodecaItems->items[i].name,defaultDoceca.items[i].name);
	}
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
