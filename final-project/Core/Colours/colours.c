/*
 * colours.c
 *
 *  Created on: 6 Jun 2022
 *      Author: GraemeGets
 */

#include "colours.h"
#include "string.h"


colour_t colours[COLOUR_COUNT_MAX] = {

			{"RED", 0xFA3232},
			{"BLUE",0x3232FA},
			{"GREEN",0x32FA32},
			{"PURPLE",0x9932CC},
			{"ORANGE",0xFA9632},
			{"YELLOW",0xFAC800},

			{"BROWN", 0x643232},
			{"TEAL",0x32C896},
			{"PEACH",0xFA9664},

			{"WHITE",0xFAFAFA},
			{"SKY",0x3296c8},
			{"BRICK",0x640032}

};

colour_t * colourFindByCode(const uint32_t code)
{

	for (uint8_t i = 0 ;i<COLOUR_COUNT_MAX;i++)
	{
		if (code == colours[i].code)
		{
			return &colours[i];
		}
	}
	return NULL;
}

colour_t *colourFindByName(const char * name )
{

	for (uint8_t i = 0 ;i<COLOUR_COUNT_MAX;i++)
	{
		if (strcmp(name,colours[i].name))
		{
			return &colours[i];
		}
	}

	return NULL;
}
