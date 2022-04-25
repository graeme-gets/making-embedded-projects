/*
 * cBuffer.c
 *
 *  Created on: Apr 25, 2022
 *      Author: GraemeGets
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
	if (cBuffer_Length(cbuffer) == cbuffer->size-1)
	{
		return CBUFFER_FULL;
	}
	cbuffer->buffer[cbuffer->write] = data;
	cbuffer->write = (cbuffer->write+1) & (cbuffer->size-1);
	return CBUFFER_OK;
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

CBUFFER_ERR cBuffer_Length(cBuffer_t* cbuffer)
{
	return (cbuffer->write - cbuffer->read) & (cbuffer->size-1);
	return CBUFFER_OK;
}

