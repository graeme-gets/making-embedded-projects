/*
 * cBuffer.c
 *
 *  Created on: Apr 25, 2022
 *      Author: GraemeGets
 *      Original : basic Code and concept from 'Making Embedded Systems by Elicia White
 */

#include "cBuffer.h"
#include "string.h"


void cBuffer_init(cBuffer_t *cbuffer,uint8_t* buffer,uint8_t size)
{
	// TODO : Use Assert to verify length of CBuffer
	cbuffer->buffer = buffer;
	cbuffer->read = 0;
	cbuffer->write = 0;
	cbuffer->size = size;
	memset(cbuffer->buffer,0, cbuffer->size);
}

CBUFFER_ERR cBuffer_Write(cBuffer_t * cbuffer, uint8_t data)
{
	cbuffer->buffer[cbuffer->write] = data;
	cbuffer->write = (cbuffer->write+1) & (cbuffer->size-1);
	return CBUFFER_OK;
}

void cBuffer_Kill(cBuffer_t* cbuffer)
{
	cbuffer->read = 0;
	cbuffer->write = 0;
	memset(cbuffer->buffer,0, cbuffer->size-1);
}

CBUFFER_ERR cBuffer_isFull(cBuffer_t* cbuffer)
{
	if (cBuffer_Length(cbuffer) == cbuffer->size-1)
	{
			return CBUFFER_FULL;
	}
	else
		return CBUFFER_OK;

}

/* Looks for a string within the buffer and fills the provided buffer with the string
 *
 */
CBUFFER_ERR cBuffer_GetString(cBuffer_t* cbuffer,uint8_t *string, uint8_t sTerminator, uint8_t* len)
{
	uint8_t i;
	uint8_t found = 0;
	uint8_t chr;
	CBUFFER_ERR err = CBUFFER_OK;
	uint8_t length;
	uint8_t read = cbuffer->read;
	uint8_t ch;
	length = cBuffer_Length(cbuffer);

	for (i=0; i < length;i++)
	{
		err = cBuffer_Peek(cbuffer, &ch, &read);

		if (ch == '\r') // A string is found - copy it to the outgoing buffer
		{
			found = 1;
			break;
		}
	}
	if (found)
	{
		// Get length of string
		*len = cBuffer_LengthCustom	(cbuffer, cbuffer->read, read);
		do{
			err = cBuffer_Read(cbuffer, &chr);
			*string = chr;
			string++;
		} while (chr != sTerminator || err != CBUFFER_OK);
	}
	else
		err = CBUFFER_NO_STRING;


	return err;
}

CBUFFER_ERR cBuffer_Read(cBuffer_t* cbuffer,uint8_t* data)
{
	if (cBuffer_Length(cbuffer) == 0)
	{
		return CBUFFER_EMPTY;
	}
	*data = cbuffer->buffer[cbuffer->read];
	cbuffer->read = (cbuffer->read+1) & (cbuffer->size-1);
	return CBUFFER_OK;
}

CBUFFER_ERR cBuffer_Peek(cBuffer_t* cbuffer,uint8_t* data, uint8_t*read)
{
	if (cBuffer_Length(cbuffer) == 0)
	{
		return CBUFFER_EMPTY;
	}
	*data = cbuffer->buffer[*read];
	*read = ((*read)+1) & (cbuffer->size-1);
	return CBUFFER_OK;
}

uint8_t cBuffer_Length(cBuffer_t* cbuffer)
{
	return (cbuffer->write - cbuffer->read) & (cbuffer->size-1);

}

uint8_t cBuffer_LengthCustom(cBuffer_t* cbuffer,uint8_t start, uint8_t end)
{
	return (end - start) & (cbuffer->size-1);
}


