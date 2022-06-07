/*
 * stringHelpers.c
 *
 *  Created on: 27 Apr 2022
 *      Author: GraemeGets
 */

/***********************************************************
 * ManageDEL : this routing walks the string to find a backspace or Delete
 * and remove the relevant characters from the string
 *
 * @param: filename GIF File name to be loaded
 * @return: Inited GIF Handler or NULL for error
 *
 ***********************************************************/

#include "main.h"
#include "string.h"
#include "stringHelpers.h"

void manageBackSpace(char* string)
{
		char *sPtr = string;
		sPtr = strchr(string,BKSP);
		while (*sPtr != 0)
		{
			// Handle if first Char is a BackSpace
			if (string[0] == BKSP)
			{
				strcpy(sPtr,sPtr+1);
			}
			// handle if char is a backspace and is at the end ont he string
			else if (*sPtr == BKSP && *(sPtr+1) == '\0')
			{
				//Just delete last character
				*(sPtr-1) = '\0';
			}
			else if (*sPtr == BKSP )
			{
				strcpy(sPtr-1,sPtr+1);
			}
			// handle is char is a backspace and IS at the end
			sPtr = strchr(string,BKSP);
		}
}

uint8_t isNumber(uint8_t *string)
{
	while (*string != 0x0)
	{
		if ((*string) < '0' || (*string) > '9' )
			return 0;
		string++;
	}
	return 1;
}


void byteToBin(uint8_t b, char*text)
{
	uint8_t pos;
	for (uint8_t cnt = 0;cnt<8;cnt++)
	{
		pos = 7-cnt;
		if (b & 1<< cnt)
			text[pos] = '1';
		else
			text[pos] = '0';

	}
	text[8] = 0x0;
}

