/*
 * colours.c
 *
 *  Created on: 6 Jun 2022
 *      Author: GraemeGets
 */

#include "colours.h"
#include "string.h"




colour_t colours[COLOUR_COUNT_MAX] = {

			{COLOUR_RED_ID,"RED", 0xFA0000},
			{COLOUR_BLUE_ID,"BLUE",0x3232FA},
			{COLOUR_GREEN_ID,"GREEN",0x32FA32},
			{COLOUR_PURPLE_ID,"PURPLE",0x9932CC},
			{COLOUR_ORANGE_ID,"ORANGE",0xFA9632},
			{COLOUR_YELLOW_ID,"YELLOW",0xFAC800},

			{COLOUR_BROWN_ID,"BROWN", 0x643232},
			{COLOUR_TEAL_ID,"TEAL",0x32C896},
			{COLOUR_PEACH_ID,"PEACH",0xFA9664},

			{COLOUR_WHITE_ID,"WHITE",0xFAFAFA},
			{COLOUR_SKY_ID,"SKY",0x3296c8},
			{COLOUR_BRICK_ID,"BRICK",0x640032},
			{COLOUR_BLACK_ID,"BLACK",0x000000},
			{COLOUR_ERROR_ID,"BLACK",0xFA0000}

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

colour_t *colourFindByid(const uint8_t id )
{

	for (uint8_t i = 0 ;i<COLOUR_COUNT_MAX;i++)
	{
		if (id == colours[i].id)
		{
			return &colours[i];
		}
	}

	return NULL;
}

